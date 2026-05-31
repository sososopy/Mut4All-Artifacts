use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Opaque_Return_Types_In_Recursive_Functions_156;

impl Mutator for Modify_Opaque_Return_Types_In_Recursive_Functions_156 {
    fn name(&self) -> &str {
        "Modify_Opaque_Return_Types_In_Recursive_Functions_156"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut opaque_type_counter = 0;
        let mut alias_definitions = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        opaque_type_counter += 1;
                        let alias_ident = Ident::new(
                            &format!("OpaqueType{}", opaque_type_counter),
                            Span::call_site(),
                        );
                        let alias_definition: Item = parse_quote! {
                            type #alias_ident = #type_impl_trait;
                        };
                        alias_definitions.push(alias_definition);
                        item_fn.sig.output = ReturnType::Type(
                            token::RArrow {
                                spans: [Span::call_site(), Span::call_site()],
                            },
                            Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath::from(alias_ident),
                            })),
                        );
                    }
                }
            }
        }
        for alias_definition in alias_definitions.into_iter().rev() {
            file.items.insert(0, alias_definition);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with opaque return types using `impl Trait`, especially those with recursive characteristics. It replaces the `impl Trait` return type with a type alias, defined at the module level, and updates the function signature to use this alias. This transformation tests the compiler's handling of opaque type aliases in recursive and complex function contexts, potentially exposing weaknesses in type resolution and inference."
    }
}