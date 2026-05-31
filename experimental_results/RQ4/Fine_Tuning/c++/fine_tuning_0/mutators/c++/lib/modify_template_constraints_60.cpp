//source file
#include "../include/Modify_Template_Constraints_60.h"

// ========================================================================================================
#define MUT60_OUTPUT 1

void MutatorFrontendAction_60::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      auto TL = TD->getTemplateParameters();
      if (TL->size() == 0)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      for (auto TP : *TL) {
        if (TP->getKind() == Decl::Kind::TemplateTypeParm) {
          auto TTP = dyn_cast<TemplateTypeParmDecl>(TP);
          if (TTP->hasTypeConstraint()) {
            auto concept_name = TTP->getTypeConstraint()
                                    ->getNamedConcept()
                                    ->getNameAsString();
            auto allconcepts = Result.Context->getTranslationUnitDecl()
                                   ->lookup(TTP->getDeclName());
            if (allconcepts.empty())
              return;
            for (auto concept : allconcepts) {
              if (concept->getNameAsString() != concept_name) {
                auto new_concept_name = concept->getNameAsString();
                llvm::outs() << new_concept_name << '\n';
                auto pos = content.find(concept_name);
                if (pos != string::npos)
                  content.replace(pos, concept_name.length(), new_concept_name);
                break;
              }
            }
          }
        } else if (TP->getKind() == Decl::Kind::NonTypeTemplateParm) {
          auto NTTP = dyn_cast<NonTypeTemplateParmDecl>(TP);
          if (NTTP->getType().getAsString().find("auto") != string::npos) {
            auto type = NTTP->getType();
            if (type->getContainedAutoType()->isConstrained()) {
              auto concept_name = type->getContainedAutoType()
                                      ->getTypeConstraintConcept()
                                      ->getNameAsString();
              auto allconcepts = Result.Context->getTranslationUnitDecl()
                                     ->lookup(NTTP->getDeclName());
              if (allconcepts.empty())
                return;
              for (auto concept : allconcepts) {
                if (concept->getNameAsString() != concept_name) {
                  auto new_concept_name = concept->getNameAsString();
                  llvm::outs() << new_concept_name << '\n';
                  auto pos = content.find(concept_name);
                  if (pos != string::npos)
                    content.replace(pos, concept_name.length(),
                                    new_concept_name);
                  break;
                }
              }
            }
          }
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_60::MutatorASTConsumer_60::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::decl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}