//source file
#include "../include/modify_union_declarations_with_attributes_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("UnionDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        if (UD->isUnion()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), UD->getSourceRange());
            if (declaration.find("__attribute__((packed))") == std::string::npos) {
                declaration += " __attribute__((packed));";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isUnion()).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}