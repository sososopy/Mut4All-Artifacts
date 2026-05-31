use crate::mutator::Mutator;
use syn::parse_quote;
use syn::Item;
use syn::Stmt;

pub struct Insert_Derive_Trait_Struct_255 {}

impl Mutator for Insert_Derive_Trait_Struct_255 {
    fn name(&self) -> &str {
        "Insert_Derive_Trait_Struct_255"
    }
    fn mutate(file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            if let block = &mut func.block {
                                if block.stmts.is_empty() {
                                    continue;
                                }
                                let has_struct = block.stmts.iter().any(|stmt| {
                                    if let Stmt::Item(item) = stmt {
                                        matches!(item, Item::Struct(_))
                                    } else {
                                        false
                                    }
                                });
                                if has_struct {
                                    continue;
                                }
                                let trait_ident = trait_path.segments.last().unwrap().ident.clone();
                                let struct_item = parse_quote! {
                                    #[derive(#trait_ident)]
                                    struct LocalStruct {}
                                };
                                block.stmts.insert(0, Stmt::Item(struct_item));
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