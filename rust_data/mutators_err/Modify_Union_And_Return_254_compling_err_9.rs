use syn::parse_quote;
use syn::Item;
use syn::ReturnType;
use syn::Type;
use syn::Stmt;
use proc_macro2::Ident;
use proc_macro2::Span;
use syn::token::Colon;
use syn::FieldsNamed;
use crate::mutator::Mutator;

pub struct Modify_Union_And_Return_254;

impl Mutator for Modify_Union_And_Return_254 {
    fn name(&self) -> &str {
        "Modify_Union_And_Return_254"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::Path(type_path) = &**return_type {
                                if type_path.path.segments.last().unwrap().ident == "Option" {
                                    if let Some(union_index) = method.block.stmts.iter().position(|stmt| {
                                        if let Stmt::Item(Item::Union(_)) = stmt {
                                            true
                                        } else {
                                            false
                                        }
                                    }) {
                                        if let Stmt::Item(Item::Union(union_item)) = &mut method.block.stmts[union_index] {
                                            if let syn::Fields::Named(fields_named) = &mut union_item.fields {
                                                let field_type = fields_named.named.iter().next().unwrap().ty.clone();
                                                let new_field = syn::Field {
                                                    attrs: vec![],
                                                    vis: syn::Visibility::Inherited,
                                                    ident: Some(Ident::new("b", Span::call_site())),
                                                    colon_token: Some(Colon::default()),
                                                    ty: field_type.clone(),
                                                    mutability: None,
                                                };
                                                fields_named.named.push(new_field);
                                                method.sig.output = parse_quote!((Option<<Self as Iterator>::Item>, Option<U>));
                                                method.block.stmts.push(parse_quote!(return (None, None);));
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
        "The mutation operator targets functions within impl blocks that use a union. It modifies the union by adding a new field of the same type, changes the function's return type to a tuple with two Option elements, and updates the function logic to return this tuple. This approach ensures broad application across different codebases by focusing on type-based matching rather than specific type names."
    }
}