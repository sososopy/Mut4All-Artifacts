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

pub struct Replace_TypeAlias_ImplTrait_With_Concrete_166;

impl Mutator for Replace_TypeAlias_ImplTrait_With_Concrete_166 {
    fn name(&self) -> &str {
        "Replace_TypeAlias_ImplTrait_With_Concrete_166"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut candidate_types = Vec::new();
        let mut impl_trait_aliases = Vec::new();
        
        struct Collector<'a> {
            types: &'a mut Vec<Type>,
            aliases: &'a mut Vec<(Ident, TypeImplTrait)>,
        }
        
        impl<'a> Visit<'a> for Collector<'a> {
            fn visit_item(&mut self, i: &'a Item) {
                if let Item::Type(alias) = i {
                    if let Type::ImplTrait(impl_trait) = &*alias.ty {
                        self.aliases.push((alias.ident.clone(), impl_trait.clone()));
                    }
                }
                if let Item::Struct(struct_item) = i {
                    if struct_item.fields.is_empty() {
                        self.types.push(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(struct_item.ident.clone()),
                        }));
                    }
                }
                syn::visit::visit_item(self, i);
            }
            
            fn visit_type(&mut self, ty: &'a Type) {
                match ty {
                    Type::Path(type_path) => {
                        if let Some(segment) = type_path.path.segments.last() {
                            if segment.ident == "u32" || segment.ident == "i32" ||
                               segment.ident == "bool" || segment.ident == "char" ||
                               segment.ident == "String" {
                                self.types.push(ty.clone());
                            }
                        }
                    }
                    Type::Tuple(tuple) => {
                        if tuple.elems.is_empty() {
                            self.types.push(Type::Tuple(syn::TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: Punctuated::new(),
                            }));
                        }
                    }
                    _ => {}
                }
                syn::visit::visit_type(self, ty);
            }
        }
        
        let mut collector = Collector {
            types: &mut candidate_types,
            aliases: &mut impl_trait_aliases,
        };
        collector.visit_file(&file);
        
        for (ident, impl_trait) in impl_trait_aliases {
            let bounds = impl_trait.bounds;
            let mut suitable_types = candidate_types.iter().filter(|ty| {
                let mut satisfies = true;
                for bound in bounds.iter() {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        let trait_name = trait_bound.path.segments.last().unwrap().ident.to_string();
                        if trait_name == "Copy" || trait_name == "Send" || trait_name == "Debug" ||
                           trait_name == "Clone" || trait_name == "ToString" {
                            if let Type::Path(type_path) = ty {
                                let type_name = type_path.path.segments.last().unwrap().ident.to_string();
                                if type_name == "u32" || type_name == "i32" ||
                                   type_name == "bool" || type_name == "char" {
                                    satisfies = true;
                                } else {
                                    satisfies = false;
                                }
                            } else if let Type::Tuple(tuple) = ty {
                                if tuple.elems.is_empty() {
                                    satisfies = true;
                                } else {
                                    satisfies = false;
                                }
                            } else {
                                satisfies = false;
                            }
                        }
                    }
                }
                satisfies
            }).cloned().collect::<Vec<_>>();
            
            if suitable_types.is_empty() {
                let mutant_name = Ident::new("MutantUnit", Span::call_site());
                let mutant_struct = ItemStruct {
                    attrs: Vec::new(),
                    vis: syn::Visibility::Inherited,
                    struct_token: token::Struct::default(),
                    ident: mutant_name.clone(),
                    generics: syn::Generics::default(),
                    fields: syn::Fields::Unit,
                    semi_token: Some(token::Semi::default()),
                };
                file.items.push(Item::Struct(mutant_struct));
                
                for bound in bounds.iter() {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        let trait_path = trait_bound.path.clone();
                        let impl_item = Item::Impl(syn::ItemImpl {
                            attrs: Vec::new(),
                            defaultness: None,
                            unsafety: None,
                            impl_token: token::Impl::default(),
                            generics: syn::Generics::default(),
                            trait_: Some((None, trait_path, syn::token::For::default())),
                            self_ty: Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath::from(mutant_name.clone()),
                            })),
                            items: Vec::new(),
                            brace_token: token::Brace::default(),
                        });
                        file.items.push(impl_item);
                    }
                }
                
                suitable_types.push(Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(mutant_name),
                }));
            }
            
            if let Some(selected_type) = suitable_types.choose(&mut thread_rng()) {
                for item in &mut file.items {
                    if let Item::Type(alias) = item {
                        if alias.ident == ident {
                            *alias.ty = selected_type.clone();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type alias declarations using `impl Trait` syntax with a concrete type that satisfies the trait bounds but changes the underlying representation. It first collects candidate concrete types from the seed program (unit structs, primitives, empty tuples). For each impl Trait alias, it filters candidates that satisfy the trait bounds (Copy, Send, Debug, Clone, ToString). If no suitable type exists, it introduces a new unit struct MutantUnit and implements the required traits for it. The concrete type is then substituted at the alias definition site. This transformation tests the compiler's handling of type alias impl Trait substitution and constraint checking, potentially causing type mismatches or inference errors."
    }
}