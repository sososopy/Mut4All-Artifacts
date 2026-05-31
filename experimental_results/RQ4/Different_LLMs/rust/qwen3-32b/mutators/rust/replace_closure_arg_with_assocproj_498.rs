use syn::parse_quote;
use quote::quote;
use crate::mutator::Mutator;
use syn::Item;
use syn::GenericParam;
use syn::TypeParamBound;
use syn::PathArguments;
use syn::punctuated::Punctuated;
use syn::PathSegment;
use syn::token::Lt;
use syn::token::Gt;
use syn::AngleBracketedGenericArguments;
use syn::Path;

pub struct Replace_Closure_Arg_With_AssocProj_498;

impl Mutator for Replace_Closure_Arg_With_AssocProj_498 {
    fn name(&self) -> &str {
        "Replace_Closure_Arg_With_AssocProj_498"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut suitable_traits = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_def) = item {
                if !trait_def.generics.lifetimes().collect::<Vec<_>>().is_empty() && has_associated_type(trait_def) {
                    let num_type_params = trait_def.generics.params.iter().filter(|param| matches!(param, GenericParam::Type(_))).count();
                    suitable_traits.push((trait_def.ident.clone(), num_type_params));
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                        let trait_name = last_segment.ident.to_string();
                                        if ["Fn", "FnMut", "FnOnce"].contains(&trait_name.as_str()) {
                                            if let PathArguments::Parenthesized(parens) = &mut last_segment.arguments {
                                                let args = &mut parens.inputs;
                                                if let Some((trait_name, num_type_params)) = suitable_traits.first() {
                                                    let projection = create_projection(trait_name, *num_type_params);
                                                    *args.first_mut().unwrap() = projection;
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn has_associated_type(trait_def: &syn::ItemTrait) -> bool {
    for item in &trait_def.items {
        if let syn::TraitItem::Type(_) = item {
            return true;
        }
    }
    false
}

fn create_projection(trait_name: &syn::Ident, num_type_params: usize) -> syn::Type {
    let mut args = Punctuated::new();
    for _ in 0..num_type_params {
        args.push(parse_quote!(i32));
    }
    let path_args = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
        colon2_token: None,
        lt_token: Lt::default(),
        args,
        gt_token: Gt::default(),
    });
    let trait_segment = PathSegment {
        ident: trait_name.clone(),
        arguments: path_args,
    };
    let trait_path = Path {
        leading_colon: None,
        segments: Punctuated::from_iter(vec![trait_segment]),
    };
    let tokens = quote! { <F as #trait_path>::Type };
    syn::parse2(tokens).unwrap()
}