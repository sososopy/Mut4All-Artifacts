use syn::parse_quote;
use crate::mutator::Mutator;
use syn::punctuated::Punctuated;
use syn::TypeParamBound;
use syn::GenericArgument;
use syn::Type;
use syn::TypePath;
use syn::AngleBracketedGenericArguments;
use syn::token::Lt;
use syn::token::Gt;
use syn::TraitBound;
use syn::TraitBoundModifier;
use syn::AssocType;

pub struct Introduce_Nested_Const_GAT_Bounds_81;

impl Mutator for Introduce_Nested_Const_GAT_Bounds_81 {
    fn name(&self) -> &str {
        "Introduce_Nested_Const_GAT_Bounds_81"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(trait_type) = trait_item {
                        let mut new_bounds = Punctuated::new();
                        for bound in &trait_type.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let const_expr: syn::Expr = parse_quote!(1i32);
                                let const_arg = GenericArgument::Const(const_expr.clone());
                                let type_b_path = parse_quote!(TypeB<{ const_expr }>);
                                let type_b_type = Type::Path(TypePath {
                                    qself: None,
                                    path: type_b_path,
                                });
                                let assoc_type = GenericArgument::AssocType(syn::AssocType {
                                    ident: syn::Ident::new("TypeB", proc_macro2::Span::call_site()),
                                    eq_token: syn::token::Eq::default(),
                                    ty: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: parse_quote!(TypeB<{ const_expr }>),
                                    })),
                                });
                                let mut trait_args = Punctuated::new();
                                trait_args.push(const_arg);
                                trait_args.push(assoc_type);
                                let trait_angle_args = AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: Lt::default(),
                                    args: trait_args,
                                    gt_token: Gt::default(),
                                };
                                let trait_b_path = syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segments = Punctuated::new();
                                        segments.push(syn::PathSegment {
                                            ident: syn::Ident::new("TraitB", proc_macro2::Span::call_site()),
                                            arguments: syn::PathArguments::AngleBracketed(trait_angle_args),
                                        });
                                        segments
                                    },
                                };
                                let new_trait_bound = TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: trait_b_path,
                                };
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        trait_type.bounds = new_bounds;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces nested const-Generic Associated Type (GAT) bounds in traits with associated types. It transforms a trait's associated type bound from a simple trait bound to a nested const-GAT bound with a constant parameter and an assigned type. This forces the compiler to resolve nested const parameters during type checking, increasing the likelihood of exposing ICEs in const-GAT resolution logic."
    }
}