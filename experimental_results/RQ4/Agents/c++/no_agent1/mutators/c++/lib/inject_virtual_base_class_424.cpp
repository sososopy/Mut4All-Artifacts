//source file
#include "../include/inject_virtual_base_class_424.h"

// ========================================================================================================
#define MUT424_OUTPUT 1

void MutatorFrontendAction_424::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;
        
        visitedClasses.insert(BaseClass);
    }
    else if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;
        if (!DerivedClass->isCompleteDefinition() || DerivedClass->isLambda())
            return;

        for (const auto *BaseClass : visitedClasses) {
            if (!DerivedClass->isDerivedFrom(BaseClass)) {
                std::string baseName = BaseClass->getNameAsString();
                std::string derivedName = DerivedClass->getNameAsString();
                std::string mutation = "class " + derivedName + " : virtual public " + baseName + " {};\n";
                mutation = "/*mut424*/" + mutation;
                Rewrite.InsertTextAfterToken(DerivedClass->getEndLoc(), mutation);
            }
        }
    }
}

void MutatorFrontendAction_424::MutatorASTConsumer_424::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}