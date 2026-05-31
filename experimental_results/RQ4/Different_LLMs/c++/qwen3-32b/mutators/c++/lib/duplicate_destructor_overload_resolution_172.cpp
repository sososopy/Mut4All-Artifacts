//source file
#include "../include/Duplicate_Destructor_Overload_Resolution_172.h"

// ========================================================================================================
#define MUT172_OUTPUT 1

void MutatorFrontendAction_172::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithDtor")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        auto className = CL->getNameAsString();
        if (className.empty())
            return;

        std::string newDtor = "~" + className + "() {}";

        auto content = stringutils::rangetoStr(*Result.SourceManager, CL->getSourceRange());
        size_t pos = content.rfind('}');
        if (pos == std::string::npos)
            return;

        content.insert(pos, "\n    " + newDtor);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);
    }
}

void MutatorFrontendAction_172::MutatorASTConsumer_172::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxDestructorDecl(isUserProvided()))).bind("ClassWithDtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}