use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Item;
use syn::ImplItem;
use syn::GenericParam;
use syn::Type;

pub struct Nested_GAT_Struct_With_Where_135;

impl Mutator for Nested_GAT_Struct_With_Where_135 {
    fn name(&self) -> &str {
        "Nested_GAT_Struct_With_Where_135"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Type(assoc_type) = impl_item {
                        if let Some(where_clause) = &assoc_type.generics.where_clause {
                            let mut lifetimes = Vec::new();
                            for param in &assoc_type.generics.params {
                                if let GenericParam::Lifetime(lifetime_param) = param {
                                    lifetimes.push(lifetime_param.lifetime.clone());
                                }
                            }
                            if let Some(lifetime) = lifetimes.first() {
                                if let Type::Path(type_path) = &*assoc_type.ty {
                                    let new_type = parse_quote! {
                                        #type_path.path<#lifetime, #assoc_type.ty>
                                    };
                                    assoc_type.ty = new_type;
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