//source file
#include "../include/Remove_Concept_Constraint_From_Class_Template_Specialization_88.h"

// ========================================================================================================
#define MUT88_OUTPUT 1

void MutatorFrontendAction_88::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassTemplateSpecialization")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto source = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto length = name.length();
      if (source.find('<') == string::npos ||
          source.find('>') == string::npos)
        return;
      auto left = source.find('<');
      auto right = source.rfind('>');
      auto params = source.substr(left + 1, right - left - 1);
      llvm::outs() << params << '\n';
      auto new_source = name + "<" + params + ">";
      llvm::outs() << new_source << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), new_source);
    }
}
  
void MutatorFrontendAction_88::MutatorASTConsumer_88::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("ClassTemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}