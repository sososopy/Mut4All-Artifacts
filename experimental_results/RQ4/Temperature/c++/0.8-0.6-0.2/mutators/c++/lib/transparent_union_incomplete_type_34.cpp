//source file
#include "../include/transparent_union_incomplete_type_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TransparentUnion")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *UT = TD->getUnderlyingType()->getAs<clang::RecordType>()) {
            if (UT->getDecl()->hasAttr<clang::TransparentUnionAttr>()) {
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
                size_t pos = declaration.find("{");
                if (pos != std::string::npos) {
                    std::string mutatedUnion = "\nunion IncompleteType member;";
                    declaration.insert(pos + 1, mutatedUnion);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
                }
            }
        }
    }
}

void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typedefDecl(hasType(asString("union"))).bind("TransparentUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}