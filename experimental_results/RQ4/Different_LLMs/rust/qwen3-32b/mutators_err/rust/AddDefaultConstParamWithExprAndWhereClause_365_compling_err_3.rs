use syn::parse_quote;
use crate::mutator::Mutator;
use syn::GenericParam;
use syn::punctuated::Punctuated;
use syn::Item;

pub struct AddDefaultConstParamWithExprAndWhereClause_365;

impl Mutator for AddDefaultConstParamWithExprAndWhereClause_365 {
    fn name(&self) -> &str {
        "AddDefaultConstParamWithExprAndWhereClause_365"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let has_const = trait_item.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                });
                if !has_const {
                    trait_item.generics.params.push(parse_quote!(const X: usize));
                    trait_item.generics.params.push(parse_quote!(const Y: usize));
                    trait_item.generics.params.push(parse_quote!(const Z: usize = { X + Y }));

                    let mut where_clause = syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    };
                    let array_type = parse_quote! { [(); X + Y] };
                    let predicate = syn::WherePredicate::Type(syn::PredicateType {
                        lifetimes: None,
                        bounded_ty: array_type,
                        colon_token: Default::default(),
                        bounds: Punctuated::new(),
                    });
                    where_clause.predicates.push(predicate);
                    trait_item.generics.where_clause = Some(where_clause);

                    let trait_ident = &trait_item.ident;
                    let impl_item = parse_quote! {
                        impl<const X: usize, const Y: usize> #trait_ident<X, Y> for MyStruct {}
                    };
                    file.items.push(Item::Impl(impl_item));

                    let has_my_struct = file.items.iter().any(|item| {
                        if let syn::Item::Struct(struct_item) = item {
                            struct_item.ident == "MyStruct"
                        } else {
                            false
                        }
                    });
                    if !has_my_struct {
                        let struct_item = parse_quote! {
                            pub struct MyStruct;
                        };
                        file.items.push(Item::Struct(struct_item));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}