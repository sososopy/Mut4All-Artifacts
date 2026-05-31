//source file
#include "../include/Insert_Template_Conversion_Operator_Call_In_Template_Class_Method_160.h"

// ========================================================================================================
#define MUT160_OUTPUT 1

void MutatorFrontendAction_160::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->isTemplated())
        return;
      if (MT->getDescribedClassTemplate() == nullptr)
        return;
      if (MT->getDescribedClassTemplate()->getTemplateParameters()->size() ==
          0)
        return;
      auto decls = MT->decls();
      bool has_template_method = false;
      for (auto decl : decls) {
        if (decl->getDescribedTemplate() != nullptr) {
          has_template_method = true;
          break;
        }
      }
      if (has_template_method == false)
        return;
      llvm::outs() << "mut160: " << MT->getNameAsString() << '\n';
      //Get the source code text of target node
      auto methods = MT->methods();
      for (auto method : methods) {
        if (method->getDescribedTemplate() != nullptr) {
          llvm::outs() << method->getNameAsString() << '\n';
        }
      }
    } else if (auto *MD =
                   Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      if (!MD->isTemplated())
        return;
      if (MD->getDescribedTemplate() == nullptr)
        return;
      llvm::outs() << "mut160: " << MD->getNameAsString() << '\n';
      //Get the source code text of target node
      auto DL = MD->getParent();
      if (!DL->isTemplated())
        return;
      auto DL_template_paras =
          DL->getDescribedClassTemplate()->getTemplateParameters();
      if (DL_template_paras->size() == 0)
        return;
      llvm::outs() << DL->getNameAsString() << '\n';
      //Perform mutation on the source code text by applying string replacement
      string ins = "0, operator int()";
      //Replace the original AST node with the mutated one
      if (MD->hasBody()) {
        auto body = MD->getBody();
        Rewrite.ReplaceText(body->getBeginLoc(), 0, "/*mut160*/" + ins + ";");
      }
    }
}
  
void MutatorFrontendAction_160::MutatorASTConsumer_160::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto template_class_matcher = cxxRecordDecl().bind("TemplateClass");
    auto template_method_matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_class_matcher, &callback);
    matchFinder.addMatcher(template_method_matcher, &callback);
    matchFinder.matchAST(Context);
}