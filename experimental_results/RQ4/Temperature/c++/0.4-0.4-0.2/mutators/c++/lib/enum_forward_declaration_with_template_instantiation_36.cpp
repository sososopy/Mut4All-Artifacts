//source file
#include "../include/enum_forward_declaration_with_template_instantiation_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("forwardEnum")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                ED->getLocation()))
            return;

        std::string enumName = ED->getNameAsString();
        std::string templateCode = "\n/*mut36*/template <typename T>\n"
                                   "struct CheckType {\n"
                                   "    static_assert(sizeof(T) > 0, \"Type must be complete\");\n"
                                   "};\n"
                                   "CheckType<" + enumName + "> checker;\n";
        
        Rewrite.InsertTextAfterToken(ED->getEndLoc(), templateCode);
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(unless(isDefinition())).bind("forwardEnum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}