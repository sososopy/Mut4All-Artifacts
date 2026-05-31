use crate::mutator::Mutator;
use syn::FnArg;
use syn::Pat;
use syn::Type;
use syn::PatType;
use syn::token::Colon;

pub struct UnboxedClosureArgTupleViolation_495;

impl Mutator for UnboxedClosureArgTupleViolation_495 {
    fn name(&self) -> &str {
        "UnboxedClosureArgTupleViolation_495"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.abi.as_ref().map_or(false, |abi| abi.name() == "rust-call") && func.sig.inputs.len() == 1 {
                    if let FnArg::Typed(pat_type) = &mut func.sig.inputs[0] {
                        if let Pat::Tuple(tuple_pat) = &*pat_type.pat {
                            if let Type::Tuple(tuple_type) = &*pat_type.ty {
                                let new_args: Vec<_> = tuple_pat.elems.iter().zip(tuple_type.elems.iter()).map(|(pat, ty)| {
                                    FnArg::Typed(PatType {
                                        pat: Box::new(Pat::clone(pat)),
                                        ty: Box::new(Type::clone(ty)),
                                        colon_token: Colon::default(),
                                        attrs: pat_type.attrs.clone(),
                                    })
                                }).collect();
                                func.sig.inputs.clear();
                                for arg in new_args {
                                    func.sig.inputs.push(arg);
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