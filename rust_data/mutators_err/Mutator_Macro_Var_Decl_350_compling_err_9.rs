use proc_macro2::{Span, TokenStream};
use quote::*;
use syn::{
    parse_quote, visit_mut::VisitMut, Expr, File, Ident, Item, Stmt,
};

use crate::mutator::Mutator;

pub struct Mutator_Macro_Var_Decl_350;

impl Mutator for Mutator_Macro_Var_Decl_350 {
    fn name(&self) -> &str {
        "Mutator_Macro_Var_Decl_350"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MacroVisitor {
            mutations: Vec<Stmt>,
        }

        impl VisitMut for MacroVisitor {
            fn visit_stmt_mut(&mut self, node: &mut Stmt) {
                if let Stmt::Macro(mac) = node {
                    if let Some(tokens) = mac.mac.tokens.clone().into_iter().next() {
                        if let Ok(mut stmt) = syn::parse2::<Stmt>(TokenStream::from(tokens)) {
                            if let Stmt::Local(local) = &mut stmt {
                                let invalid_ident: Ident = parse_quote! { var };
                                local.attrs.push(syn::Attribute {
                                    pound_token: syn::token::Pound {
                                        spans: [Span::call_site()],
                                    },
                                    style: syn::AttrStyle::Outer,
                                    bracket_token: syn::token::Bracket {
                                        span: syn::token::Bracket {
                                            span: Span::call_site(),
                                        },
                                    },
                                    meta: syn::Meta::Path(parse_quote!(#invalid_ident)),
                                });
                                self.mutations.push(Stmt::Local(local.clone()));
                            }
                        }
                    }
                }
                syn::visit_mut::visit_stmt_mut(self, node);
            }
        }

        let mut visitor = MacroVisitor { mutations: Vec::new() };
        visitor.visit_file_mut(file);
        if !visitor.mutations.is_empty() {
            file.items.push(Item::Verbatim(quote! { #(#visitor.mutations)* }));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations with variable declarations. It locates macros defined to accept statements or expressions and replaces valid variable declarations with invalid ones by omitting the keyword 'let' and using an undefined keyword like 'var'. This introduces semantic errors, causing the parser to misbehave."
    }
}