//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Function_Template_Instantiation_243
 */ 
class MutatorFrontendAction_243 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(243)

private:
    class MutatorASTConsumer_243 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_243(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *last_struct = nullptr;
    };
};

//source file
#include "../include/nested_function_template_instantiation_243.h"

// ========================================================================================================
#define MUT243_OUTPUT 1

void MutatorFrontendAction_243::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      last_struct = CL;
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "FunctionTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (last_struct == nullptr)
        return;
      if (last_struct->getEndLoc() < FT->getEndLoc())
        return;
      auto insts = FT->specializations();
      if (insts.empty())
        return;
      auto last_inst = insts.back();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             last_inst->getSourceRange());
      auto template_parms = FT->getTemplateParameters();
      if (template_parms->size() > 1) {
        auto parm1 = template_parms->getParam(0);
        auto parm2 = template_parms->getParam(1);
        auto name1 = parm1->getNameAsString();
        auto name2 = parm2->getNameAsString();
        llvm::outs() << name1 << " " << name2 << '\n';
        stringutils::strrreplace(content, name1, name2);
      } else {
        stringutils::strrreplace(content, template_parms->getParam(0)->getNameAsString(), "int");
      }
      content = "/*mut243*/" + content;
      Rewrite.ReplaceText(last_inst->getEndLoc(), 0, content);
    }
}
  
void MutatorFrontendAction_243::MutatorASTConsumer_243::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto func_template_matcher = functionTemplateDecl().bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_template_matcher, &callback);
    matchFinder.matchAST(Context);
}