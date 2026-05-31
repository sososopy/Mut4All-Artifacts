use syn::parse_quote;
use crate::mutator::Mutator;
use syn::{Item, Stmt, Expr, GenericParam, ConstParam, Type, TypePath};
use proc_macro2::{TokenTree, Ident, Span, TokenStream};
use syn::token;

pub struct Mutator_Async_Unsafe_Asm_Const_316;

impl Mutator for Mutator_Async_Unsafe_Asm_Const_316 {
    fn name(&self) -> &str {
        "Async_Unsafe_Asm_Const_316"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() && item_fn.sig.unsafety.is_some() {
                    let mut found_asm = false;
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Macro(expr_macro), _) = stmt {
                            if expr_macro.mac.path.is_ident("asm") {
                                found_asm = true;
                                if let Some(tt) = expr_macro.mac.tokens.clone().into_iter().find(|tt| match tt {
                                    TokenTree::Ident(ident) => ident == "const",
                                    _ => false,
                                }) {
                                    let const_param = Ident::new("M", Span::call_site());
                                    item_fn.sig.generics.params.push(GenericParam::Const(
                                        ConstParam {
                                            attrs: Vec::new(),
                                            const_token: token::Const(tt.span()),
                                            ident: const_param.clone(),
                                            colon_token: token::Colon(tt.span()),
                                            ty: Type::Path(TypePath {
                                                qself: None,
                                                path: parse_quote!(usize),
                                            }),
                                            eq_token: None,
                                            default: None,
                                        },
                                    ));
                                    let new_tokens: TokenStream = expr_macro.mac.tokens.clone().into_iter().map(|tt| {
                                        if let TokenTree::Ident(ident) = &tt {
                                            if ident == "const" {
                                                return TokenTree::Ident(const_param.clone());
                                            }
                                        }
                                        tt
                                    }).collect();
                                    expr_macro.mac.tokens = new_tokens;
                                }
                            }
                        }
                    }
                    if found_asm {
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async unsafe functions using inline assembly with constant expressions. It introduces a const parameter in the function signature and replaces the existing non-declared const operand in the asm! macro with the new const parameter. This mutation operator is designed to trigger potential issues with constant parameters in async and unsafe contexts."
    }
}