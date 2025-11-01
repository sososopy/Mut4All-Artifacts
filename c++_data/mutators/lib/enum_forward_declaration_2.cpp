//source file
#include "../include/enum_forward_declaration_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;
        
        if (ED->isComplete() || visitedEnums.find(ED) != visitedEnums.end())
            return;

        visitedEnums.insert(ED);

        std::string enumName = ED->getNameAsString();
        if (enumName.empty())
            return;

        std::string mutation = "template <typename T> void someFunction(typename T::EnumMember); someFunction<" + enumName + ">(0);";
        mutation = "/*mut2*/" + mutation;

        Rewrite.InsertTextAfterToken(ED->getEndLoc(), "\n" + mutation);
    }
}

void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(unless(isDefinition())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}