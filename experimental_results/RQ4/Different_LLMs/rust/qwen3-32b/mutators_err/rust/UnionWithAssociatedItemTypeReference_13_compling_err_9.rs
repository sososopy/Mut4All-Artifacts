use syn::Token;
use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Ident;
use proc_macro2::Span;
use syn::punctuated::Punctuated;

pub struct UnionWithAssociatedItemTypeReference_13;

impl Mutator for UnionWithAssociatedItemTypeReference_13 {
    fn name(&self) -> &str {
        "UnionWithAssociatedItemTypeReference_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, ref trait_path, _)) = impl_item.trait_ {
                    let trait_name = trait_path.segments.last().unwrap().ident.clone();
                    let self_ty = &impl_item.self_ty;
                    let union = syn::ItemUnion {
                        attrs: vec![],
                        vis: syn::Visibility::Inherited,
                        ident: Ident::new("U", Span::call_site()),
                        union_token: <Token![union]>::default(),
                        generics: parse_quote!(()),
                        fields: syn::Fields::Named(syn::FieldsNamed {
                            brace_token: syn::token::Brace::default(),
                            named: {
                                let mut fields: Punctuated<syn::Field, Token![,]> = Punctuated::new();
                                fields.push(syn::Field {
                                    attrs: vec![],
                                    vis: syn::Visibility::Inherited,
                                    mutability: syn::FieldMutability::None,
                                    ident: Some(Ident::new("field", Span::call_site())),
                                    colon_token: Some(Default::default()),
                                    ty: parse_quote!(<#self_ty as #trait_name>::Item),
                                });
                                fields
                            },
                        }),
                    };
                    file.items.push(syn::Item::Union(union));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}