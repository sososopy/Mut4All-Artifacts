//source file
#include "../include/Insert_Virtual_Base_Class_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
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
                std::string insertion = "virtual public " + baseName;
                std::string replacement = derivedName + " : " + insertion;

                auto range = DerivedClass->getSourceRange();
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
                size_t pos = originalText.find(derivedName);
                if (pos != std::string::npos) {
                    originalText.insert(pos + derivedName.length(), " : " + insertion);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), originalText);
                }
            }
        }
    }
}

void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}