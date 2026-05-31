//source file
#include "../include/insert_virtual_base_class_377.h"

// ========================================================================================================
#define MUT377_OUTPUT 1

void MutatorFrontendAction_377::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;

        if (Base->isCompleteDefinition() && !Base->isLambda()) {
            baseClasses.insert(Base->getNameAsString());
        }
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;

        if (Derived->isCompleteDefinition() && !Derived->isLambda() && !baseClasses.empty()) {
            std::string virtualBase = *baseClasses.begin();
            std::string insertion = " : virtual public " + virtualBase;
            Rewrite.InsertTextAfterToken(Derived->getBeginLoc(), insertion);
            baseClasses.erase(virtualBase);
        }
    }
}

void MutatorFrontendAction_377::MutatorASTConsumer_377::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}