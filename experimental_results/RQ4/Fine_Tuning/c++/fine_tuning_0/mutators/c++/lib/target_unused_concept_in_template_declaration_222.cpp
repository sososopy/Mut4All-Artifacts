//source file
#include "../include/Target_Unused_Concept_In_Template_Declaration_222.h"

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getKind() != Decl::Kind::ClassTemplate &&
          DL->getKind() != Decl::Kind::FunctionTemplate &&
          DL->getKind() != Decl::Kind::TypeAliasTemplate)
        return;
      auto params = DL->getTemplateParameters();
      for (auto param : *params) {
        if (param->getKind() == Decl::Kind::TemplateTypeParm) {
          auto TTP = dyn_cast<TemplateTypeParmDecl>(param);
          if (TTP->hasTypeConstraint()) {
            auto constraint = TTP->getTypeConstraint();
            auto constraint_type = constraint->getNamedConcept();
            auto concept_name = constraint_type->getName();
            llvm::outs() << concept_name << '\n';
            auto DL_str = stringutils::rangetoStr(
                *(Result.SourceManager), DL->getSourceRange());
            DL_str.insert(DL_str.find(';'), "<" + concept_name.str() + ">");
            DL_str.insert(DL_str.rfind(';'), "(" + concept_name.str() + ")");
            DL_str = "/*mut222*/" + DL_str;
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(DL->getSourceRange()), DL_str);
          }
        }
      }
    }
  }

void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::decl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}