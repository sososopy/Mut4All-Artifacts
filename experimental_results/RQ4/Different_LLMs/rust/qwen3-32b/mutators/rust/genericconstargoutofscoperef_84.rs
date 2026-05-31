use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Ident;
use syn::visit_mut::VisitMut;
use syn::ExprCall;
use syn::Expr;
use syn::PathArguments;
use syn::GenericArgument;
use rand::thread_rng;
use rand::prelude::SliceRandom;

pub struct GenericConstArgOutofScopeRef_84;

impl Mutator for GenericConstArgOutofScopeRef_84 {
    fn name(&self) -> &str {
        "GenericConstArgOutofScopeRef_84"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut functions_with_consts = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let const_params: Vec<_> = func
                    .sig
                    .generics
                    .params
                    .iter()
                    .filter_map(|p| {
                        if let syn::GenericParam::Const(c) = p {
                            Some(c.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if !const_params.is_empty() {
                    functions_with_consts.push((func.sig.ident.clone(), const_params));
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = CallVisitor {
                    functions_with_consts: &functions_with_consts,
                };
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const expressions in generic arguments of function calls with references to const parameters from other functions in the same module. This introduces out-of-scope const parameters, potentially causing type errors or ICEs in generic argument resolution."
    }
}

struct CallVisitor<'a> {
    functions_with_consts: &'a Vec<(Ident, Vec<Ident>)>,
}

impl<'a> VisitMut for CallVisitor<'a> {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(func_path) = &mut *node.func {
            let called_func_name = func_path.path.segments.last().unwrap().ident.clone();
            if let Some(last_segment) = func_path.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    for arg in args.args.iter_mut() {
                        if let GenericArgument::Const(expr) = arg {
                            let available_funcs: Vec<_> = self.functions_with_consts
                                .iter()
                                .filter(|(name, _)| name != &called_func_name)
                                .collect();
                            if !available_funcs.is_empty() {
                                let chosen_func = available_funcs.choose(&mut thread_rng()).unwrap();
                                let const_name = chosen_func.1.choose(&mut thread_rng()).unwrap().clone();
                                let new_expr = parse_quote! { #const_name };
                                *expr = new_expr;
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}