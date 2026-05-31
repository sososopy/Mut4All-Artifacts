use syn::{parse_quote, File, GenericParam, Item, ReturnType, Type, TypePath};
use crate::mutator::Mutator;

pub struct Change_Const_Generic_To_Integer_389;

impl Mutator for Change_Const_Generic_To_Integer_389 {
    fn name(&self) -> &str {
        "Change_Const_Generic_To_Integer_389"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let generics = &mut item_fn.sig.generics;
                for param in generics.params.iter_mut() {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.last().unwrap().ident == "MyStr" {
                                const_param.ty = Type::Path(TypePath {
                                    qself: None,
                                    path: parse_quote!(usize),
                                });
                                if let ReturnType::Type(_, ret_type) = &mut item_fn.sig.output {
                                    *ret_type = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: parse_quote!(usize),
                                    }));
                                }
                                item_fn.block.stmts = vec![parse_quote! {
                                    S + 1;
                                }];
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with constant generic parameters that are references to structs implementing ConstParamTy. The mutation changes the type of these parameters to an integer type (usize), modifies the return type to match, and updates the function body to handle the integer type. This explores constant evaluation and type inference in the Rust compiler."
    }
}