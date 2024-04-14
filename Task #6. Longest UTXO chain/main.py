import random
import string
import hashlib
from ecdsa import SigningKey, VerifyingKey, SECP256k1

OP_PUSH_DATA = range(0x01, 0x4e)
OP_DUP = 0x76
OP_HASH160 = 0xa9
OP_EQUALVERIFY = 0x88
OP_CHECKSIG = 0xac

def eval_script(script, stack, transaction, input_index):
    for op in script:
        if op in OP_PUSH_DATA:
            stack.append(op)
        elif op == OP_DUP:  # OP_DUP
            if not stack:
                return False
            stack.append(stack[-1])
        elif op == OP_HASH160:  # OP_HASH160
            if len(stack) < 1:
                return False
            element = stack.pop()
            stack.append(hash160(element))
        elif op == OP_EQUALVERIFY:  # OP_EQUALVERIFY
            if len(stack) < 2:
                return False
            element1 = stack.pop()
            element2 = stack.pop()
            if element1 != element2:
                return False
        elif op == OP_CHECKSIG:  # OP_CHECKSIG
            if len(stack) < 2:
                return False
            public_key = stack.pop()
            signature = stack.pop()
            tx_hash = transaction.calculate_hash()
            if not verify_signature(signature, public_key, tx_hash, input_index):
                return False
        else:
            return False

    if len(stack) == 0:
        return False

    return stack[-1] == 0x01

def hash160(data):
    sha256_hash = hashlib.sha256(data).digest()
    ripemd160_hash = hashlib.new('ripemd160', sha256_hash).digest()
    return ripemd160_hash

def verify_signature(signature, public_key, tx_hash, input_index):
    vk = VerifyingKey.from_string(bytes.fromhex(public_key), curve=SECP256k1)
    try:
        vk.verify_digest(signature, tx_hash, sigdecode=ecdsa.util.sigdecode_der)
        return True
    except ecdsa.keys.BadSignatureError:
        return False

class UTXO:
    def __init__(self, transaction_id, index, amount, script_pubkey):
        self.transaction_id = transaction_id
        self.index = index
        self.amount = amount
        self.script_pubkey = script_pubkey

class TxInput:
    def __init__(self, transaction_id, index, script_sig):
        self.transaction_id = transaction_id
        self.index = index
        self.script_sig = script_sig

class TxOutput:
    def __init__(self, amount, script_pubkey):
        self.amount = amount
        self.script_pubkey = script_pubkey

class Transaction:
    def __init__(self, transaction_id, inputs, outputs, locktime=0):
        self.transaction_id = transaction_id
        self.inputs = inputs
        self.outputs = outputs
        self.locktime = locktime

    def sign(self, private_keys):
        for i, tx_input in enumerate(self.inputs):
            private_key = private_keys[i]
            signing_key = SigningKey.from_string(bytes.fromhex(private_key), curve=SECP256k1)

            tx_hash = self.calculate_hash()
            signature = signing_key.sign_digest(tx_hash, sigencode=ecdsa.util.sigencode_der_canonize)

            script_sig = b''
            script_sig += signature
            script_sig += bytes.fromhex(signing_key.get_verifying_key().to_string().hex())

            tx_input.script_sig = script_sig

    def verify_signatures(self):
        for tx_input in self.inputs:
            script_sig = tx_input.script_sig
            signature_len = script_sig[1]
            signature = script_sig[2:2 + signature_len]
            public_key = script_sig[2 + signature_len:]

            verifying_key = VerifyingKey.from_string(public_key, curve=SECP256k1)

            tx_hash = self.calculate_hash()
            try:
                verifying_key.verify_digest(signature, tx_hash, sigdecode=ecdsa.util.sigdecode_der)
            except ecdsa.keys.BadSignatureError:
                return False

        return True
    
    def calculate_hash(self):
        tx_data = b''
        tx_data += self.encode_inputs()
        tx_data += self.encode_outputs()
        tx_data += self.locktime.to_bytes(4, byteorder='little')

        return sha256(sha256(tx_data).digest()).digest()

    def encode_inputs(self):
        inputs_data = bytes()
        for tx_input in self.inputs:
            inputs_data += bytes.fromhex(tx_input.transaction_id)
            inputs_data += tx_input.index.to_bytes(4, byteorder='little')
            script_len = len(tx_input.script_sig)
            inputs_data += script_len.to_bytes(1, byteorder='little')
            inputs_data += tx_input.script_sig
        return inputs_data

    def encode_outputs(self):
        outputs_data = bytes()
        for tx_output in self.outputs:
            outputs_data += tx_output.amount.to_bytes(8, byteorder='little')
            script_len = len(tx_output.script_pubkey)
            outputs_data += script_len.to_bytes(1, byteorder='little')
            outputs_data += tx_output.script_pubkey
        return outputs_data

    def calculate_hash(self):
        tx_data = b''
        tx_data += self.encode_inputs()
        tx_data += self.encode_outputs()
        tx_data += self.locktime.to_bytes(4, byteorder='little')

        return hashlib.sha256(hashlib.sha256(tx_data).digest()).digest()

class Block:
    def __init__(self, block_id, transactions):
        self.block_id = block_id
        self.transactions = transactions

class Blockchain:
    def __init__(self):
        self.blocks = []
        self.utxo_set = {}
        self.validated_blocks = {}

    def update_utxo_set(self, block):
        for tx in block.transactions:
            for input_utxo in tx.inputs:
                self.utxo_set.pop(f"{input_utxo.transaction_id}-{input_utxo.index}", None)
            for output in tx.outputs:
                key = f"{output.transaction_id}-{output.index}"
                self.utxo_set[key] = output

    @staticmethod
    def calculate_chain_length(chain):
        length = 0
        for block in chain:
            length += len(block.transactions)
        return length

    def find_longest_valid_chain(self):
        longest_valid_chain = []
        max_chain_length = 0

        print("Searching for the longest valid chain...")

        for block in self.blocks:
            print(f"Evaluating block {block.block_id}")
            current_chain = [block]
            utxo_set = self.validated_blocks.get(block.block_id, {}).copy()

            chain_length = self.calculate_chain_length(current_chain)
            print(f"Valid chain of length {chain_length} found")
            if chain_length > max_chain_length:
                max_chain_length = chain_length
                longest_valid_chain = current_chain
                print(f"New longest chain found, length: {max_chain_length}")

        print("Longest valid chain search completed")
        return longest_valid_chain

    def validate_block(self, block, utxo_set):
        print(f"Validating block {block.block_id}")
        for tx in block.transactions:
            if not self.validate_transaction(tx, utxo_set):
                print(f"Transaction {tx.transaction_id} in block {block.block_id} is invalid")
                return False
        return True

    def validate_transaction(self, tx, utxo_set):
        print(f"Validating transaction {tx.transaction_id}")
        input_sum = 0
        output_sum = 0

        for i, input_utxo in enumerate(tx.inputs):
            key = f"{input_utxo.transaction_id}-{input_utxo.index}"
            if key not in utxo_set:
                print(f"Input UTXO {key} not found in UTXO set")
                return False

            prev_output = utxo_set[key]
            input_sum += prev_output.amount

            stack = []
            if not eval_script(input_utxo.script_sig, stack, tx, i):
                print(f"Script evaluation failed for input UTXO {key}")
                return False

            if not eval_script(prev_output.script_pubkey, stack, tx, i):
                print(f"Script evaluation failed for output UTXO {key}")
                return False

        for output in tx.outputs:
            output_sum += output.amount

        if input_sum < output_sum:
            print(f"Transaction {tx.transaction_id} has insufficient input value")
            return False

        for input_utxo in tx.inputs:
            key = f"{input_utxo.transaction_id}-{input_utxo.index}"
            utxo_set.pop(key, None)

        for output in tx.outputs:
            key = f"{output.transaction_id}-{output.index}"
            utxo_set[key] = output

        return True

    def filter_transactions(self):
        filtered_transactions = []

        for block in self.blocks:
            for tx in block.transactions:
                if len(tx.inputs) == 1 and len(tx.outputs) == 2:
                    filtered_transactions.append(tx)

        return filtered_transactions

def get_random_num():
    return random.randint(0, 1000)

def get_random_id():
    return ''.join(random.choices(string.ascii_letters + string.digits, k=8))

def create_p2pkh_script_pubkey(public_key):
    script_pubkey = bytes([OP_DUP, OP_HASH160, len(public_key)]) + public_key + bytes([OP_EQUALVERIFY, OP_CHECKSIG])
    return script_pubkey

if __name__ == "__main__":
    blockchain = Blockchain()

    block_id_1 = get_random_num()
    tx_id_1 = get_random_id()
    tx_id_2 = get_random_id()
    amount_1 = get_random_num()
    amount_2 = get_random_num()
    amount_3 = get_random_num()
    amount_4 = get_random_num()

    block_id_2 = get_random_num()
    tx_id_4 = get_random_id()
    tx_id_5 = get_random_id()
    tx_id_6 = get_random_id()
    amount_5 = get_random_num()
    amount_6 = get_random_num()
    amount_7 = get_random_num()

    block_id_3 = get_random_num()
    tx_id_7 = get_random_id()
    tx_id_8 = get_random_id()
    amount_8 = get_random_num()
    amount_9 = get_random_num()
    amount_10 = get_random_num()

    block_id_4 = get_random_num()
    tx_id_9 = get_random_id()
    tx_id_10 = get_random_id()
    amount_11 = get_random_num()
    amount_12 = get_random_num()
    amount_13 = get_random_num()

    script_pubkey_1 = create_p2pkh_script_pubkey(bytes.fromhex("03c96d495bdfb9e3e72a9c2d5430bf5a6d1244508ce0ca67d576f37964c8382eda"))
    script_sig_1 = bytes.fromhex("47304402202d63e5e8d8590b1199e246410825daa4e3dbbb0ef6a7647af720a14b0fb5ca4f022011493011ed4f1ce0799e164898e1b4875554cceb6ce9cba21af98db0394154af01")

    script_pubkey_2 = create_p2pkh_script_pubkey(bytes.fromhex("0286c9adcb1d11791c9bd92fdf5031bae542afbdfac35528de5c54463a8fe743e8"))
    script_sig_2 = bytes.fromhex("48304502210088828c0bdfcdca68d8ae1f2761f84a64bfcf34cc117d7f7911e2512e445e7cfa02200b1b92f45eeb78c88638c8bd6837f487bc59501d90c02607fdfd2eefbf9d871401")


    block_1 = Block(
        block_id_1,
        [
            Transaction(
                tx_id_1,
                [UTXO("input1", 0, amount_1, script_pubkey_1)],
                [UTXO(tx_id_1, 0, amount_2, script_pubkey_2), UTXO(tx_id_1, 1, amount_3, "script_pubkey_3")]
            ),
            Transaction(
                tx_id_5,
                [UTXO("input4", 1, amount_7, script_pubkey_1), UTXO("input1", 4, amount_3, script_pubkey_2)],
                [UTXO(tx_id_5, 2, amount_4, "script_pubkey_6"), UTXO(tx_id_5, 4, amount_1, "script_pubkey_7"), UTXO(tx_id_6, 2, amount_2, "script_pubkey_8")]
            )
        ]
    )

    block_2 = Block(
        block_id_2,
        [
            Transaction(
                tx_id_4,
                [UTXO("input3", 2, amount_5, "script_pubkey_9")],
                [UTXO(tx_id_4, 5, amount_6, "script_pubkey_10")]
            ),
            Transaction(
                tx_id_2,
                [UTXO("input2", 0, amount_4, script_pubkey_1)],
                [UTXO(tx_id_2, 0, amount_1, script_pubkey_1), UTXO(tx_id_2, 1, amount_2, script_pubkey_2)]
            )
        ]
)


    block_3 = Block(
        block_id_3,
        [
            Transaction(
                tx_id_7,
                [UTXO(tx_id_2, 1, amount_2, "script_pubkey_14")],
                [UTXO(tx_id_7, 0, amount_8, "script_pubkey_15"), UTXO(tx_id_7, 1, amount_9, "script_pubkey_16")]
            ),
            Transaction(
                tx_id_8,
                [UTXO(tx_id_1, 1, amount_3, "script_pubkey_17")],
                [UTXO(tx_id_8, 0, amount_10, "script_pubkey_18")]
            )
        ]
    )

    block_4 = Block(
        block_id_4,
        [
            Transaction(
                tx_id_9,
                [UTXO(tx_id_7, 1, amount_9, "script_pubkey_19")],
                [UTXO(tx_id_9, 0, amount_11, "script_pubkey_20"), UTXO(tx_id_9, 1, amount_12, "script_pubkey_21")]
            ),
            Transaction(
                tx_id_10,
                [UTXO(tx_id_4, 5, amount_6, "script_pubkey_22"), UTXO(tx_id_8, 0, amount_10, "script_pubkey_23")],
                [UTXO(tx_id_10, 0, amount_13, "script_pubkey_24")]
            )
        ]
    )

    blockchain.blocks.append(block_1)
    blockchain.blocks.append(block_2)
    blockchain.blocks.append(block_3)
    blockchain.blocks.append(block_4)

    longest_chain = blockchain.find_longest_valid_chain()

    print("Longest Valid Chain:")
    for block in longest_chain:
        print(f"BlockID: {block.block_id}")
        for tx in block.transactions:
            print(f"\tTransactionID: {tx.transaction_id}")
            print("\tInputs:")
            for input_utxo in tx.inputs:
                print(f"\t\tTransactionID: {input_utxo.transaction_id}, Index: {input_utxo.index}")
            print("\tOutputs:")
            for output in tx.outputs:
                print(f"\t\tTransactionID: {output.transaction_id}, Index: {output.index}, Amount: {output.amount:.2f}, Script PubKey: {output.script_pubkey}")

    filtered_transactions = blockchain.filter_transactions()

    print("Filtered Transactions:")
    for tx in filtered_transactions:
        print(f"TransactionID: {tx.transaction_id}")
        print("\tInputs:")
        for input_utxo in tx.inputs:
            print(f"\t\tTransactionID: {input_utxo.transaction_id}, Index: {input_utxo.index}")
        print("\tOutputs:")
        for output in tx.outputs:
            print(f"\t\tTransactionID: {output.transaction_id}, Index: {output.index}, Amount: {output.amount:.2f}, Script PubKey: {output.script_pubkey}") 
