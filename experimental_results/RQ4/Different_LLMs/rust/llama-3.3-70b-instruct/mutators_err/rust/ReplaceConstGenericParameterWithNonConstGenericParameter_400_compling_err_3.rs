use syn::{File, Item, GenericParam, TypeParam};
use crate::mutator::Mutator;

struct ReplaceConstGenericParameterWithNonConstGenericParameter_400;

impl Mutator for ReplaceConstGenericParameterWithNonConstGenericParameter_400 {
    fn name(&self) -> &str {
        "ReplaceConstGenericParameterWithNonConstGenericParameter_400"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                if let Some(generics) = &mut struct_def.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            *param = GenericParam::Type(TypeParam {
                                attrs: param.attrs.clone(),
                                ident: param.ident.clone(),
                                colon_token: None,
                                bounds: Default::default(),
                                default: None,
                                eq_token: None,
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const generic parameters in struct definitions. This transformation tests the handling of non-const generic parameters in the Rust compiler and may help expose bugs related to the generic_const_exprs feature."
    }
}