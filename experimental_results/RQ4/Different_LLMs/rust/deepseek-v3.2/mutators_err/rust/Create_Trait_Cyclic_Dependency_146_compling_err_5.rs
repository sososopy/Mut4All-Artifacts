use proc_macro2::TokenStream;
use quote::quote;
use syn::{Item, ItemTrait, parse_file};
mod mutator;
use mutator::Create_Trait_Cyclic_Dependency_146;
fn main() {
    let code = r#"
        trait A {}
        trait B {}
    "#;
    let mut file_ast = parse_file(code).unwrap();
    for item in &mut file_ast.items {
        if let Item::Trait(trait_item) = item {
            Create_Trait_Cyclic_Dependency_146.mutate(&mut file_ast);
        }
    }
    let mutated = quote!(#file_ast);
    println!("{}", mutated);
}