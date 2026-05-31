//source file
#include "../include/forward_declared_enum_with_type_traits_424.h"

// ========================================================================================================
#define MUT424_OUTPUT 1

void MutatorFrontendAction_424::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("ForwardDeclaredEnum")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
            return;

        if (!ED->isCompleteDefinition()) {
            std::string enumName = ED->getNameAsString();
            std::string replacement = "enum " + enumName + " : int;";

            Rewrite.ReplaceText(ED->getSourceRange(), replacement);

            std::string staticAssert = "static_assert(!std::is_enum<" + enumName + ">::value, \"\");";
            Rewrite.InsertTextAfterToken(ED->getEndLoc(), "\n/*mut424*/" + staticAssert);
        }
    }
}
  
void MutatorFrontendAction_424::MutatorASTConsumer_424::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(unless(isDefinition())).bind("ForwardDeclaredEnum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}