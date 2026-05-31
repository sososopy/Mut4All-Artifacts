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

pub struct Create_Trait_Cyclic_Dependency_146;

impl Mutator for Create_Trait_Cyclic_Dependency_146 {
    fn name(&self) -> &str {
        "Create_Trait_Cyclic_Dependency_146"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = Vec::new();
        let mut trait_items = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                trait_names.push(trait_item.ident.clone());
                trait_items.push(trait_item);
            }
        }
        if trait_names.len() < 2 {
            return;
        }
        let mut rng = thread_rng();
        trait_names.shuffle(&mut rng);
        let t1 = trait_names[0];
        let t2 = trait_names[1];
        let mut found_t1_method_with_t2 = false;
        let mut t1_item: Option<&mut ItemTrait> = None;
        for item in trait_items.iter_mut() {
            if item.ident == t1 {
                t1_item = Some(item);
                for method in &mut item.items {
                    if let syn::TraitItem::Fn(method_item) = method {
                        let mut visitor = TypeVisitor {
                            target_trait: t2.clone(),
                            found: false,
                        };
                        visitor.visit_type(&method_item.sig.output);
                        for input in &method_item.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                visitor.visit_type(&*pat_type.ty);
                            }
                        }
                        if visitor.found {
                            found_t1_method_with_t2 = true;
                            break;
                        }
                    }
                }
                if found_t1_method_with_t2 {
                    break;
                }
            }
        }
        if !found_t1_method_with_t2 {
            return;
        }
        for item in trait_items.iter_mut() {
            if item.ident == t2 {
                let has_methods = item.items.iter().any(|item| matches!(item, syn::TraitItem::Fn(_)));
                if !has_methods {
                    let new_method: syn::TraitItem = parse_quote! {
                        fn create_cycle(param: #t1) -> #t1;
                    };
                    item.items.push(new_method);
                } else {
                    for method in &mut item.items {
                        if let syn::TraitItem::Fn(method_item) = method {
                            let param_count = method_item.sig.inputs.len();
                            let has_return_type = match &method_item.sig.output {
                                ReturnType::Default => false,
                                ReturnType::Type(_, _) => true,
                            };
                            if param_count > 0 {
                                let param_index = rng.gen_range(0..param_count);
                                if let FnArg::Typed(pat_type) = &mut method_item.sig.inputs[param_index] {
                                    pat_type.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path::from(t1.clone()),
                                    }));
                                }
                            } else if has_return_type {
                                method_item.sig.output = ReturnType::Type(
                                    token::RArrow::default(),
                                    Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path::from(t1.clone()),
                                    })),
                                );
                            } else {
                                method_item.sig.output = ReturnType::Type(
                                    token::RArrow::default(),
                                    Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path::from(t1.clone()),
                                    })),
                                );
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator creates cyclic dependencies between traits via method signatures. It identifies two traits T1 and T2 in the seed program. If T1 has a method that uses T2 as a parameter or return type, it modifies a method in T2 to use T1 as a parameter or return type. If T2 has no methods, it adds a new method with T1 as both parameter and return type. This transformation creates mutual trait dependencies, potentially triggering object safety issues, trait resolution cycles, or compiler ICEs during type checking."
    }
}

struct TypeVisitor {
    target_trait: Ident,
    found: bool,
}

impl Visit<'_> for TypeVisitor {
    fn visit_type(&mut self, node: &'_ Type) {
        if let Type::Path(type_path) = node {
            if type_path.path.is_ident(&self.target_trait) {
                self.found = true;
            }
        }
        syn::visit::visit_type(self, node);
    }
}