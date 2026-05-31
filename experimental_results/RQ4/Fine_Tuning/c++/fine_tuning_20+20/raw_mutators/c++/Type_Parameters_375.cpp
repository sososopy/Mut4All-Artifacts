//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_with_Non-Type_Parameters_375
 */ 
class MutatorFrontendAction_375 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(375)

private:
    class MutatorASTConsumer_375 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_375(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/template_instantiation_with_non-type_parameters_375.h"

// ========================================================================================================
#define MUT375_OUTPUT 1

void MutatorFrontendAction_375::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (TD->isCompleteDefinition() == false)
        return;
      if (TD->getTemplateParameters()->size() != 1)
        return;
      auto param = TD->getTemplateParameters()->getParam(0);
      if (param->isTemplateParameter() == false)
        return;
      auto TTP = dyn_cast<TemplateTypeParmDecl>(param);
      if (TTP == nullptr)
        return;
      auto template_content =
          stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      auto template_name = TD->getNameAsString();
      llvm::outs() << template_name << '\n';
      auto new_content = "/*mut375*/";
      new_content += "template <typename T> struct " + template_name +
                     " { static const T w = 0; };";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()),
                          new_content);
      cur_templates.push_back(TD);
    } else if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Structs")) { // If there is a return 0 statement in the
                                 // function, replace the return value with the
                                 // inserted static const
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getBeginLoc()))
        return;
      if (SD->isCompleteDefinition() == false)
        return;
      auto decls = SD->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          return;
        }
      }
      if (cur_templates.empty())
        return;
      size_t index = getrandom::getRandomIndex(cur_templates.size() - 1);
      auto target = cur_templates[index];
      auto ins = "/*mut375*/using Y = " + target->getNameAsString() + "<int>::w;";
      Rewrite.ReplaceText(SD->getEndLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_375::MutatorASTConsumer_375::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = classTemplateDecl().bind("Templates");
    auto struct_matcher = cxxRecordDecl(isStruct()).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}