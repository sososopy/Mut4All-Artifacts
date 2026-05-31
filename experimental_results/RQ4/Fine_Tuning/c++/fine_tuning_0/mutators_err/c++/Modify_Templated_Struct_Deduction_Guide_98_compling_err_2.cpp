//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Templated_Struct_Deduction_Guide_98
 */ 
class MutatorFrontendAction_98 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(98)

private:
    class MutatorASTConsumer_98 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_98(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Modify_Templated_Struct_Deduction_Guide_98.h"

// ========================================================================================================
#define MUT98_OUTPUT 1

void MutatorFrontendAction_98::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateStruct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isStruct() && MT->isTemplated()) {
        //Get the source code text of target node
        auto name = MT->getNameAsString();
        llvm::outs() << name << '\n';
        auto dedu_guides = MT->getDecls();
        for (auto guide : dedu_guides) {
          if (guide->getKind() == clang::Decl::Kind::CXXDeductionGuide) {
            auto dedu_guide = dyn_cast<CXXDeductionGuideDecl>(guide);
            auto params = dedu_guide->parameters();
            for (auto param : params) {
              if (param->getType()->isConstantArrayType()) {
                //Perform mutation on the source code text by applying string replacement
                Rewrite.ReplaceText(dedu_guide->getBeginLoc(), 0, "/*mut98*/");
                break;
              }
            }
          }
        }
      }
    }
    //Check whether the matched AST node is the target node
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = VD->getType();
      if (type->isStructureType()) {
        auto name = type.getAsString();
        llvm::outs() << name << '\n';
        if (name.find('<') == string::npos) {
          auto init = VD->getInit();
          if (init->getStmtClass() == clang::Stmt::StmtClass::CXXConstructExprClass) {
            auto construct = dyn_cast<CXXConstructExpr>(init);
            auto args = construct->getArgs();
            string arg_str = "";
            for (auto arg : args) {
              arg_str +=
                  stringutils::rangetoStr(*(Result.SourceManager), arg->getSourceRange());
              arg_str += ",";
            }
            if (arg_str.back() == ',')
              arg_str.pop_back();
            //Perform mutation on the source code text by applying string replacement
            Rewrite.ReplaceText(VD->getBeginLoc(), 0, "/*mut98*/" + name + "<" + arg_str + "> ");
          }
        }
      }
    }
}
  
void MutatorFrontendAction_98::MutatorASTConsumer_98::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("TemplateStruct");
    auto var_matcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}