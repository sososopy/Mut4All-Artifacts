//source file
#include "../include/insert_virtual_base_class_408.h"

// ========================================================================================================
#define MUT408_OUTPUT 1

void MutatorFrontendAction_408::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (!Base->isCompleteDefinition() || Base->isLambda())
            return;
        base_classes.insert(Base);
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (!Derived->isCompleteDefinition() || Derived->isLambda())
            return;
        
        for (const auto *Base : base_classes) {
            if (!Derived->isDerivedFrom(Base)) {
                std::string baseName = Base->getNameAsString();
                std::string derivedName = Derived->getNameAsString();
                std::string virtualBaseInsertion = "class " + derivedName + " : virtual public " + baseName + " {};\n";
                Rewrite.InsertTextAfterToken(Derived->getEndLoc(), ";\n/*mut408*/" + virtualBaseInsertion);
            }
        }
    }
}

void MutatorFrontendAction_408::MutatorASTConsumer_408::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}