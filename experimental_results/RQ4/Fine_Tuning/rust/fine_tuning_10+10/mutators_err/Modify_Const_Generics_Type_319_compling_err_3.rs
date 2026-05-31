use crate::mutator::Mutator;

pub struct Modify_Const_Generics_Type_319;

impl Mutator for Modify_Const_Generics_Type_319 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_Type_319"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let generics = &mut item_fn.sig.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "usize" {
                                    const_param.ty = parse_quote! { u32 };
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator focuses on modifying the const generics in function signatures. It identifies functions with const generics and changes their type from one numeric type to another (e.g., from usize to u32). This requires adjusting the function body and any usages to maintain type compatibility. The mutation ensures that the const generic type remains valid and tests the compiler's robustness against such changes."
    }
}