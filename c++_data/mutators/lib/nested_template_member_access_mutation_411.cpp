//source file
#include "../include/Nested_Template_Member_Access_Mutation_411.h"

// ========================================================================================================
#define MUT411_OUTPUT 1

void MutatorFrontendAction_411::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;

        if (MT->hasBody()) {
            auto bodyRange = MT->getBody()->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);

            for (const auto &pair : nestedTemplateDeclarations) {
                const CXXRecordDecl *recordDecl = pair.first;
                std::string nestedTemplateDecl = pair.second;

                if (bodyText.find(recordDecl->getNameAsString()) != std::string::npos) {
                    std::string insertText = "\n/*mut411*/" + nestedTemplateDecl;
                    Rewrite.InsertTextAfterToken(recordDecl->getEndLoc(), insertText);
                }
            }
        }
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Record")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
            return;

        if (RD->isCompleteDefinition()) {
            std::string nestedTemplateDecl = "template <class W> struct C { int MEM; };";
            nestedTemplateDeclarations[RD] = nestedTemplateDecl;
        }
    }
}

void MutatorFrontendAction_411::MutatorASTConsumer_411::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher methodMatcher = cxxMethodDecl().bind("Method");
    DeclarationMatcher recordMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("Record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}