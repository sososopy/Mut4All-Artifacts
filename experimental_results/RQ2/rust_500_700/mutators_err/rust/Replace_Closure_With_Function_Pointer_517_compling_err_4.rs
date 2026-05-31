use proc_macro2::{Span, TokenStream};
use quote::quote;
use syn::{
    Expr, File, FnArg, Ident, ItemFn, Pat, PatType, Type, TypePath, parse_quote,
    spanned::Spanned, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Replace_Closure_With_Function_Pointer_517;

impl Mutator for Replace_Closure_With_Function_Pointer_517 {
    fn name(&self) -> &str {
        "Replace_Closure_With_Function_Pointer_517"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct ClosureReplacer;

        impl VisitMut for ClosureReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Closure(closure) = expr {
                    if let Some(FnArg::Typed(PatType { ty, .. })) = closure.inputs.first() {
                        if let Type::Path(TypePath { path, .. }) = &**ty {
                            let fn_name = Ident::new("replaced_function", Span::call_site());
                            let fn_decl: ItemFn = parse_quote! {
                                fn #fn_name(#closure) {
                                    #closure.body
                                }
                            };
                            *expr = parse_quote! {
                                #fn_name as fn(#path)
                            };
                        }
                    }
                }

                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut replacer = ClosureReplacer;
        replacer.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}