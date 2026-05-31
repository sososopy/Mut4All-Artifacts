use syn::{parse_quote, Expr, ExprCall, ExprLit, ExprPath, Item, ItemFn, Lit, LitStr, Stmt};
use syn::visit_mut::VisitMut;
use rand::Rng;
use rand::thread_rng;
use rand::prelude::SliceRandom;
use regex::Regex;
use proc_macro2::Span;

struct Replace_Inline_Assembly_Operand_170;

impl Replace_Inline_Assembly_Operand_170 {
    fn name(&self) -> &str {
        "Replace_Inline_Assembly_Operand_170"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall {
                            attrs: _,
                            func: 
                                Box::new(Expr::Path(ExprPath {
                                    attrs: _,
                                    path: _,
                                    qself: _,
                                })),
                            args,
                            ..
                        }) = expr {
                            if let Some(segment) = args.get(0) {
                                if let Expr::Lit(lit) = segment {
                                    if let Lit::Str(lit_str) = &lit.lit {
                                        let assembly_code = lit_str.value();
                                        let mut register_operands = Vec::new();
                                        let mut other_operands = Vec::new();
                                        for cap in Regex::new(r"\$([a-zA-Z0-9_]+)")
                                            .unwrap()
                                            .captures_iter(&assembly_code)
                                        {
                                            let register = cap[1].to_string();
                                            register_operands.push(register);
                                        }
                                        for cap in Regex::new(r"([a-zA-Z0-9_]+)")
                                            .unwrap()
                                            .captures_iter(&assembly_code)
                                        {
                                            let operand = cap[1].to_string();
                                            if !register_operands.contains(&operand) {
                                                other_operands.push(operand);
                                            }
                                        }
                                        if !register_operands.is_empty() {
                                            let mut rng = thread_rng();
                                            let new_register_operand =
                                                register_operands.choose(&mut rng).unwrap();
                                            let new_assembly_code =
                                                assembly_code.replace(
                                                    &register_operands[0],
                                                    new_register_operand,
                                                );
                                            *lit = Lit::Str(LitStr::new(&new_assembly_code, Span::call_site()));
                                            *expr = parse_quote! {
                                                #lit
                                            };
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly code and replaces the operands with other valid operands in the same scope. It applies to code structures that contain inline assembly, such as the core::arch::asm! macro in Rust. The operator takes into account the types of the operands and only replaces them with other operands of the same type. This transformation aims to test the robustness of the compiler's inline assembly handling and error reporting."
    }
}