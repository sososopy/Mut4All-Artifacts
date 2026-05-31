//source file
#include "../include/Mutator_template_function_specialization_with_empty_type_packs_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      auto ClassName = TD->getNameAsString();
      std::string MutationText = "template <unsigned N> static auto get() -> decltype(std::declval<" + ClassName + "<>>()::template get<N>()) { return {}; }";
      
      for (auto *Spec : TD->specializations()) {
          if (Spec->getTemplateArgs().size() == 0) {
              SourceLocation InsertLoc = Spec->getEndLoc();
              Rewrite.InsertTextAfterToken(InsertLoc, "\n/*mut9*/" + MutationText);
          }
      }
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(hasName("Example")).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}