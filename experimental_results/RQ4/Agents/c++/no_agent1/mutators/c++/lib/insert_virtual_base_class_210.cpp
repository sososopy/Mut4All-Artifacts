//source file
#include "../include/insert_virtual_base_class_210.h"

// ========================================================================================================
#define MUT210_OUTPUT 1

void MutatorFrontendAction_210::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (!Base->isCompleteDefinition() || Base->isLambda())
            return;

        std::string baseName = Base->getNameAsString();
        if (baseName.empty() || visitedClasses.count(baseName))
            return;

        visitedClasses.insert(baseName);
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (!Derived->isCompleteDefinition() || Derived->isLambda())
            return;

        std::string derivedName = Derived->getNameAsString();
        if (derivedName.empty() || visitedClasses.count(derivedName))
            return;

        for (const auto &baseName : visitedClasses) {
            std::string mutation = "class Mut_210 : virtual public " + baseName + " {};\n";
            Rewrite.InsertTextAfterToken(Derived->getEndLoc(), "\n/*mut210*/" + mutation);
        }
    }
}

void MutatorFrontendAction_210::MutatorASTConsumer_210::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}