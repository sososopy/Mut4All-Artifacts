//source file
#include "../include/inheritance_with_constructor_specialization_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        //Filter nodes in header files
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (!Base->hasDefaultConstructor())
            return;
        BaseClass = Base;
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (!BaseClass || !Derived->isDerivedFrom(BaseClass))
            return;
        if (Derived->ctor_begin() != Derived->ctor_end())
            return; // Ensure no additional constructors

        //Perform mutation on the source code text by applying string replacement
        std::string usingDeclaration = "public:\n\tusing " + BaseClass->getNameAsString() + "::" + BaseClass->getNameAsString() + ";\n";
        Rewrite.InsertTextAfterToken(Derived->getEndLoc(), "/*mut4*/" + usingDeclaration);
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher baseMatcher = cxxRecordDecl(has(cxxConstructorDecl(isDefaultConstructor()))).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}