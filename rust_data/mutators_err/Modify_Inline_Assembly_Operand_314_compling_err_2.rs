use crate::mutator::Mutator;

pub struct Modify_Inline_Assembly_Operand_314;

impl Mutator for Modify_Inline_Assembly_Operand_314 {
    fn name(&self) -> &str {
        "Modify_Inline_Assembly_Operand_314"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Macro(ref mut mac)) = stmt {
                        if mac.mac.path.is_ident("asm") {
                            let mut tokens = mac.mac.tokens.clone().into_iter();
                            let mut new_tokens = TokenStream::new();
                            while let Some(token) = tokens.next() {
                                if let TokenTree::Literal(ref lit) = token {
                                    let lit_str = lit.to_string();
                                    if lit_str.contains("global.get 0") {
                                        let new_lit = lit_str.replace("global.get 0", "global.get 1");
                                        new_tokens.extend(quote! { #new_lit });
                                    } else {
                                        new_tokens.extend(quote! { #lit });
                                    }
                                } else {
                                    new_tokens.extend(quote! { #token });
                                }
                            }
                            mac.mac.tokens = new_tokens;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets inline assembly blocks in Rust code. It identifies inline assembly statements using the `asm!` macro and modifies the operand within the assembly instruction. Specifically, it searches for the operand `global.get 0` and replaces it with `global.get 1`, or another valid operand. This mutation explores the effects of changing assembly instructions on code generation, ensuring the modified operand still adheres to the syntax and semantics of the target architecture's assembly language."
    }
}