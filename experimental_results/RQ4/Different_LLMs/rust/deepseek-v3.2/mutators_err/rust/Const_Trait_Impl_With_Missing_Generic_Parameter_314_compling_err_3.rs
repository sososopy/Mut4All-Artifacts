use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Const_Trait_Impl_With_Missing_Generic_Parameter_314;

impl Mutator for Const_Trait_Impl_With_Missing_Generic_Parameter_314 {
    fn name(&self) -> &str {
        "Const_Trait_Impl_With_Missing_Generic_Parameter_314"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut concrete_types = Vec::new();
        let mut std_traits = HashSet::new();
        std_traits.insert("Clone".to_string());
        std_traits.insert("Copy".to_string());
        std_traits.insert("PartialEq".to_string());
        std_traits.insert("Eq".to_string());
        std_traits.insert("PartialOrd".to_string());
        std_traits.insert("Ord".to_string());
        std_traits.insert("Hash".to_string());
        std_traits.insert("Default".to_string());
        std_traits.insert("Debug".to_string());
        std_traits.insert("Display".to_string());
        std_traits.insert("From".to_string());
        std_traits.insert("Into".to_string());
        std_traits.insert("TryFrom".to_string());
        std_traits.insert("TryInto".to_string());
        std_traits.insert("AsRef".to_string());
        std_traits.insert("AsMut".to_string());
        std_traits.insert("Borrow".to_string());
        std_traits.insert("BorrowMut".to_string());
        std_traits.insert("ToOwned".to_string());
        std_traits.insert("FromStr".to_string());
        std_traits.insert("ToString".to_string());
        std_traits.insert("Iterator".to_string());
        std_traits.insert("IntoIterator".to_string());
        std_traits.insert("DoubleEndedIterator".to_string());
        std_traits.insert("ExactSizeIterator".to_string());
        std_traits.insert("FusedIterator".to_string());
        std_traits.insert("Future".to_string());
        std_traits.insert("Fn".to_string());
        std_traits.insert("FnMut".to_string());
        std_traits.insert("FnOnce".to_string());
        std_traits.insert("Sized".to_string());
        std_traits.insert("Unpin".to_string());
        std_traits.insert("UnwindSafe".to_string());
        std_traits.insert("RefUnwindSafe".to_string());

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                concrete_types.push(item_struct.ident.to_string());
            }
            if let Item::Enum(item_enum) = item {
                concrete_types.push(item_enum.ident.to_string());
            }
            if let Item::Type(item_type) = item {
                concrete_types.push(item_type.ident.to_string());
            }
        }

        if concrete_types.is_empty() {
            concrete_types.push("i32".to_string());
            concrete_types.push("String".to_string());
        }

        let mut needs_const_trait_attr = false;
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.defaultness.is_some() {
                    let trait_path = if let Some((_, path, _)) = &item_impl.trait_ {
                        path
                    } else {
                        continue;
                    };
                    let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                    let is_std_trait = std_traits.contains(&trait_name);

                    let mut has_undefined_generic = false;
                    if let Type::Path(type_path) = &*item_impl.self_ty {
                        if type_path.path.segments.len() == 1 {
                            let ident = &type_path.path.segments[0].ident;
                            if ident.to_string().chars().all(|c| c.is_ascii_uppercase()) && ident.to_string().len() == 1 {
                                let generic_name = ident.to_string();
                                let mut found = false;
                                for param in &item_impl.generics.params {
                                    if let GenericParam::Type(type_param) = param {
                                        if type_param.ident.to_string() ==generic_name {
                                            found = true;
                                            break;
                                        }
                                    }
                                }
                                if !found {
                                    has_undefined_generic = true;
                                }
                            }
                        }
                    }

                    if has_undefined_generic {
                        let mut rng = thread_rng();
                        let chosen_type = concrete_types.choose(&mut rng).unwrap();
                        item_impl.self_ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new(chosen_type, Span::call_site())),
                        }));
                    }

                    if !is_std_trait {
                        needs_const_trait_attr = true;
                    } else {
                        item_impl.defaultness = None;
                    }
                }
            }
        }

        if needs_const_trait_attr {
            let dummy_trait = Item::Trait(ItemTrait {
                attrs: vec![Attribute {
                    style: AttrStyle::Outer,
                    meta: Meta::List(MetaList {
                        path: SynPath::from(Ident::new("const_trait", Span::call_site())),
                        delimiter: MacroDelimiter::Paren(Paren::default()),
                        tokens: TokenStream::new(),
                    }),
                    pound_token: token::Pound::default(),
                    bracket_token: token::Bracket::default(),
                }],
                trait_token: token::Trait::default(),
                ident: Ident::new("DummyConstTrait", Span::call_site()),
                generics: Generics::default(),
                colon_token: None,
                supertraits: Punctuated::new(),
                items: vec![],
                unsafety: None,
                auto_token: None,
                restriction: None,
                brace_token: token::Brace::default(),
                vis: syn::Visibility::Public(syn::VisPublic {
                    pub_token: token::Pub::default(),
                }),
            });
            file.items.insert(0, dummy_trait);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const trait implementations with missing generic parameters or lacking const trait attribute. It scans for impl blocks with const keyword, checks if the implementing type is an undefined single-letter generic, and replaces it with a concrete type from the seed program or introduces i32/String. For non-standard library traits, it adds a dummy #[const_trait] trait definition; for standard traits, it removes the const keyword. This transformation creates syntactically valid but semantically unusual impl blocks, aiming to trigger edge cases in const trait handling and effects feature interactions."
    }
}