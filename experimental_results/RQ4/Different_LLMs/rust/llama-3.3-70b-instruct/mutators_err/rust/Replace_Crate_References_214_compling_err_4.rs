use rand::thread_rng;
use rand::Rng;
use syn::{Item, Ident, Visibility, UseTree, UseName, UsePath, UseRename, UseGlob};
use proc_macro2::Span;
use syn::token;

struct Replace_Crate_References_214;

impl Replace_Crate_References_214 {
    fn name(&self) -> &str {
        "Replace_Crate_References_214"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut new_crate_name = String::new();
        let mut rng = thread_rng();
        for _ in 0..10 {
            new_crate_name.push(rng.gen_range('a'..='z'));
        }

        for item in &mut file.items {
            if let Item::Use(use_stmt) = item {
                if let UseTree::Path(path) = &mut use_stmt.tree {
                    path.ident = Ident::new(&new_crate_name, path.ident.span());
                } else if let UseTree::Name(name) = &mut use_stmt.tree {
                    name.ident = Ident::new(&new_crate_name, name.ident.span());
                } else if let UseTree::Rename(rename) = &mut use_stmt.tree {
                    rename.ident = Ident::new(&new_crate_name, rename.ident.span());
                } else if let UseTree::Glob(glob) = &mut use_stmt.tree {
                    // No ident field in UseGlob, skip
                }
            } else if let Item::ExternCrate(extern_crate) = item {
                extern_crate.ident = Ident::new(&new_crate_name, extern_crate.ident.span());
            }
        }

        // Add a new extern crate declaration for the new crate
        file.items.push(Item::ExternCrate(syn::ItemExternCrate {
            attrs: Vec::new(),
            vis: Visibility::Inherited,
            extern_token: token::Extern { span: Span::call_site() },
            crate_token: token::Crate { span: Span::call_site() },
            ident: Ident::new(&new_crate_name, Span::call_site()),
            rename: None,
            semi_token: token::Semi { span: Span::call_site() },
        }));
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing crate references with new ones to test the compiler's ability to resolve crate metadata and handle crate references correctly. It introduces new crate references in use statements and extern crate declarations, and adds a new extern crate declaration for the new crate."
    }
}