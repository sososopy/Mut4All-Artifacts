//source file
#include "../include/Mutator_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Template")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isThisDeclarationADefinition() == false)
        return;
      if (MT->getTemplateParameters()->size() == 0)
        return;
      if (MT->getTemplatedDecl()->getIdentifier() == nullptr)
        return;
      llvm::outs() << "mut61: " << MT->getNameAsString() << '\n';
      auto template_params = MT->getTemplateParameters();
      if (template_params->size() == 0)
        return;
      for (auto param : *template_params) {
        if (auto named_decl = dyn_cast<NamedDecl>(param)) {
          llvm::outs() << named_decl->getNameAsString() << '\n';
        }
      }
      auto concept = template_params->getParam(0);
      if (concept->getKind() != Decl::Kind::TemplateTypeParm)
        return;
      auto concept_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                 concept->getSourceRange());
      llvm::outs() << concept_str << '\n';
      if (concept_str.find("requires") == string::npos)
        return;
      // if(concept_str.find("std::")==string::npos) return;
      auto concept_begin = concept_str.find("std::");
      auto concept_end = concept_str.find('<');
      if (concept_begin == string::npos)
        return;
      if (concept_end == string::npos)
        concept_end = concept_str.length();
      auto concept_name =
          concept_str.substr(concept_begin, concept_end - concept_begin);
      llvm::outs() << concept_name << '\n';
      auto new_concept_name = "std::same_as";
      auto new_concept_str = concept_str.replace(
          concept_begin, concept_end - concept_begin, new_concept_name);
      llvm::outs() << new_concept_str << '\n';
      Rewrite.ReplaceText(concept->getSourceRange(), new_concept_str);
    }
}
  
void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}