//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Pack_Expansion_In_Alias_Template_Context_3
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)

private:
    class MutatorASTConsumer_3 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_classes;
    };
};

//source file
#include "../include/Template_Parameter_Pack_Expansion_In_Alias_Template_Context_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Classes")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      if (CT->getTemplateParameters()->size() == 0)
        return;
      if (CT->getTemplateParameters()->getParam(0)->isTemplateParameterPack() ==
          false)
        return;
      if (CT->getTemplatedDecl()->isCompleteDefinition() == false)
        return;
      llvm::outs() << CT->getNameAsString() << '\n';
      cur_classes.push_back(CT);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>( "Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile( FD->getLocation())) return;
      if (cur_classes.size() == 0)
        return;
      auto target = cur_classes[getrandom::getRandomIndex(cur_classes.size() - 1)];
      llvm::outs() << "target: " << target->getNameAsString() << '\n';
      auto target_name = target->getNameAsString();
      auto content = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      auto tp = target->getTemplateParameters();
      llvm::outs() << "tp size: " << tp->size() << '\n';
      std::string alias_name = "Mut_3_" + target_name;
      std::string alias_decl = "/*mut3*/template<";
      for (int i = 0; i < tp->size(); ++i) {
        if (i != 0)
          alias_decl += ",";
        alias_decl += "typename ";
        alias_decl += tp->getParam(i)->getNameAsString();
      }
      alias_decl += "> using " + alias_name + "=" + target_name + "<";
      for (int i = 0; i < tp->size(); ++i) {
        if (i != 0)
          alias_decl += ",";
        alias_decl += tp->getParam(i)->getNameAsString();
      }
      alias_decl += ">;\n";
      llvm::outs() << alias_decl << '\n';
      Rewrite.ReplaceText(FD->getBeginLoc(), 0, alias_decl);
      if (target->getTemplatedDecl()->isCompleteDefinition() == false)
        return;
      auto DL = target->getTemplatedDecl();
      auto DL_content = stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      DL_content += "/*mut3*/\n";
      DL_content += "template<typename U> void foo();\n";
      Rewrite.ReplaceText(DL->getSourceRange(), DL_content);
      string spec_decl = "template<typename... Ts> template<typename U> void " +
                         target_name + "<Ts...>::foo(){\n";
      spec_decl += alias_name + "<Ts...,U> x;\n}\n";
      llvm::outs() << spec_decl << '\n';
      Rewrite.ReplaceText(DL->getEndLoc(), 0, spec_decl);
      string ins = "/*mut3*/" + alias_name + "<int,double,char> x;\n";
      ins += "x.template foo<float>();\n";
      Rewrite.ReplaceText(FD->getEndLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Classes");
    auto func_matcher = functionDecl(hasName("main")).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}