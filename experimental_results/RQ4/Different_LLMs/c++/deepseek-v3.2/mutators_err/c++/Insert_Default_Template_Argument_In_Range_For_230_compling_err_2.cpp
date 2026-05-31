//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Default_Template_Argument_In_Range_For_230
 */ 
class MutatorFrontendAction_230 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(230)

private:
    class MutatorASTConsumer_230 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_230(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl*> candidateTemplates;
        std::vector<std::string> candidateDefaultArgs;
    };
};

//source file
#include "../include/Mutator_Insert_Default_Template_Argument_In_Range_For_230.h"

// ========================================================================================================
#define MUT230_OUTPUT 1

void MutatorFrontendAction_230::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      candidateTemplates.push_back(TD);
    }
    else if (auto *FR = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("ForRangeStmt")) {
      //Filter nodes in header files
      if (!FR || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FR->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto *RangeExpr = FR->getRangeInit();
      if (!RangeExpr)
        return;
      //Analyze the range expression to find template instantiations
      const clang::Expr* Expr = RangeExpr->IgnoreImpCasts();
      if (const clang::DeclRefExpr* DRE = llvm::dyn_cast<clang::DeclRefExpr>(Expr)) {
        const clang::ValueDecl* VD = DRE->getDecl();
        if (const clang::VarDecl* Var = llvm::dyn_cast<clang::VarDecl>(VD)) {
          const clang::Type* Type = Var->getType().getTypePtr();
          if (const clang::TemplateSpecializationType* TST = llvm::dyn_cast<clang::TemplateSpecializationType>(Type)) {
            const clang::TemplateDecl* TD = TST->getTemplateName().getAsTemplateDecl();
            //Find corresponding template declaration
            for (const clang::TemplateDecl* CandTD : candidateTemplates) {
              if (CandTD == TD) {
                //Extract template arguments from the instantiation
                std::string DefaultArg;
                if (TST->getNumArgs() > 0) {
                  const clang::TemplateArgument& Arg = TST->getArg(0);
                  if (Arg.getKind() == clang::TemplateArgument::Type) {
                    DefaultArg = Arg.getAsType().getAsString();
                  } else if (Arg.getKind() == clang::TemplateArgument::Integral) {
                    DefaultArg = std::to_string(Arg.getAsIntegral().getSExtValue());
                  } else if (Arg.getKind() == clang::TemplateArgument::Expression) {
                    const clang::Expr* ExprArg = Arg.getAsExpr();
                    DefaultArg = stringutils::rangetoStr(*(Result.SourceManager), ExprArg->getSourceRange());
                  }
                }
                if (!DefaultArg.empty()) {
                  candidateDefaultArgs.push_back(DefaultArg);
                }
                break;
              }
            }
          }
        }
      }
    }
    else if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Check if this template parameter belongs to a candidate template
      const clang::DeclContext* DC = TP->getDeclContext();
      if (const clang::TemplateDecl* TD = llvm::dyn_cast<clang::TemplateDecl>(DC)) {
        bool isCandidate = false;
        for (const clang::TemplateDecl* CandTD : candidateTemplates) {
          if (CandTD == TD) {
            isCandidate = true;
            break;
          }
        }
        if (!isCandidate)
          return;
        //Ensure we have a default argument candidate
        if (candidateDefaultArgs.empty())
          return;
        //Select a default argument
        size_t index = getrandom::getRandomIndex(candidateDefaultArgs.size() - 1);
        std::string DefaultArg = candidateDefaultArgs[index];
        //Perform mutation on the source code text by applying string replacement
        std::string ParamText = stringutils::rangetoStr(*(Result.SourceManager), TP->getSourceRange());
        if (TP->hasDefaultArgument())
          return; //Already has a default argument
        //Insert default argument
        ParamText += " = " + DefaultArg;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TP->getSourceRange()), ParamText);
      }
    }
}
  
void MutatorFrontendAction_230::MutatorASTConsumer_230::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    StatementMatcher forRangeMatcher = clang::ast_matchers::cxxForRangeStmt().bind("ForRangeStmt");
    DeclarationMatcher paramMatcher = clang::ast_matchers::templateTypeParmDecl().bind("TemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(forRangeMatcher, &callback);
    matchFinder.addMatcher(paramMatcher, &callback);
    matchFinder.matchAST(Context);
}