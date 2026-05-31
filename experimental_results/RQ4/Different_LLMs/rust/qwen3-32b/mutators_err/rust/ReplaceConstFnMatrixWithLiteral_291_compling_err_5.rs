use syn::parse_quote;
use syn::ExprArray;
use syn::Expr;
use syn::token::Bracket;
use syn::punctuated::Punctuated;
use crate::mutator::Mutator;

pub struct ReplaceConstFnMatrixWithLiteral_291;

impl Mutator for ReplaceConstFnMatrixWithLiteral_291 {
    fn name(&self) -> &str {
        "ReplaceConstFnMatrixWithLiteral_291"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                    for segment in &mut type_path.path.segments {
                        if let syn::PathArguments::AngleBracketed(angle_args) = &mut segment.arguments {
                            for arg in &mut angle_args.args {
                                if let syn::GenericArgument::Const(expr) = arg {
                                    if let syn::Expr::Call(call_expr) = &*expr {
                                        if let syn::Expr::Path(path_expr) = &*call_expr.func {
                                            if let Some(ident) = path_expr.path.get_ident() {
                                                if ident == "to_matrix" {
                                                    let array_expr = {
                                                        let zero_expr: syn::Expr = parse_quote!(0);
                                                        let inner_array = syn::ExprArray {
                                                            attrs: vec![],
                                                            bracket_token: syn::token::Bracket::default(),
                                                            elems: Punctuated::from_iter(vec![zero_expr]),
                                                        };
                                                        let outer_array = syn::ExprArray {
                                                            attrs: vec![],
                                                            bracket_token: syn::token::Bracket::default(),
                                                            elems: Punctuated::from_iter(vec![syn::Expr::Array(inner_array)]),
                                                        };
                                                        outer_array
                                                    };
                                                    *expr = syn::Expr::Array(array_expr);
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}