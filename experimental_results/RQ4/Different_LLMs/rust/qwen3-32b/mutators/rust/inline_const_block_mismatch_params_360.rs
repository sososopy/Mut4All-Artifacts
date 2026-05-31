use crate::mutator::Mutator;

pub struct Inline_Const_Block_Mismatch_Params_360;

impl Mutator for Inline_Const_Block_Mismatch_Params_360 {
    fn name(&self) -> &str {
        "Inline_Const_Block_Mismatch_Params_360"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                let path = &mut trait_bound.path;
                                if let Some(last_segment) = path.segments.last_mut() {
                                    if let syn::PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                        for arg in &mut args.args {
                                            if let syn::GenericArgument::Const(const_arg) = arg {
                                                if let syn::Expr::Call(call_expr) = const_arg {
                                                    if call_expr.args.len() > 0 {
                                                        call_expr.args.pop();
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
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_pred) = predicate {
                            for bound in &mut type_pred.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let path = &mut trait_bound.path;
                                    if let Some(last_segment) = path.segments.last_mut() {
                                        if let syn::PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            for arg in &mut args.args {
                                                if let syn::GenericArgument::Const(const_arg) = arg {
                                                    if let syn::Expr::Call(call_expr) = const_arg {
                                                        if call_expr.args.len() > 0 {
                                                            call_expr.args.pop();
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const function calls within trait bounds and alters the number of arguments in the call, using existing variables from the seed program. This creates unresolved const expressions during trait evaluation, potentially exposing ICEs in the compiler's generic const evaluation logic."
    }
}