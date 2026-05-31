//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Templated_Lambda_Explicit_Operator_Call_39
 */ 
class MutatorFrontendAction_39 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(39)

private:
    class MutatorASTConsumer_39 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_39(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Templated_Lambda_Explicit_Operator_Call_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXOperatorCallExpr>("opCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Ensure it's a call to operator() on a lambda
      if (CE->getOperator() != clang::OO_Call) return;
      
      auto *Callee = CE->getCallee()->IgnoreParenImpCasts();
      if (!Callee) return;
      
      // Check if it's a member operator call: obj.operator()
      auto *ME = clang::dyn_cast<clang::MemberExpr>(Callee);
      if (!ME) return;
      
      // Verify the member is named operator()
      auto *MD = clang::dyn_cast<clang::CXXMethodDecl>(ME->getMemberDecl());
      if (!MD || !MD->isOverloadedOperator() || MD->getOverloadedOperator() != clang::OO_Call) return;
      
      // Check if the call has explicit template arguments
      auto *DRE = clang::dyn_cast<clang::DeclRefExpr>(ME->getBase()->IgnoreParenImpCasts());
      if (!DRE) return;
      
      auto *VD = clang::dyn_cast<clang::VarDecl>(DRE->getDecl());
      if (!VD) return;
      
      // Check if variable is a lambda
      auto LambdaType = VD->getType()->getAs<clang::RecordType>();
      if (!LambdaType || !LambdaType->getDecl()->isLambda()) return;
      
      // Get template arguments if present
      auto *D = clang::dyn_cast<clang::FunctionDecl>(MD);
      if (!D) return;
      
      const clang::TemplateArgumentList *TAL = nullptr;
      if (auto *Spec = clang::dyn_cast<clang::FunctionTemplateSpecializationInfo>(D->getTemplateSpecializationInfo())) {
        TAL = &Spec->TemplateArguments;
      }
      if (!TAL || TAL->size() == 0) return;
      
      //Get the source code text of target node
      clang::SourceRange FullRange = CE->getSourceRange();
      clang::SourceLocation StartLoc = ME->getBeginLoc();
      clang::SourceLocation EndLoc = CE->getEndLoc();
      
      // Extract the lambda variable name
      std::string LambdaName = VD->getNameAsString();
      if (LambdaName.empty()) return;
      
      // Build template arguments string
      std::string TemplateArgs;
      clang::LangOptions LangOpts;
      clang::PrintingPolicy Policy(LangOpts);
      llvm::raw_string_ostream OS(TemplateArgs);
      OS << "<";
      for (unsigned I = 0; I < TAL->size(); ++I) {
        if (I > 0) OS << ", ";
        TAL->get(I).print(Policy, OS);
      }
      OS << ">";
      OS.flush();
      
      // Get function arguments
      std::string FuncArgs;
      llvm::raw_string_ostream ArgsOS(FuncArgs);
      for (unsigned I = 0; I < CE->getNumArgs(); ++I) {
        if (I > 0) ArgsOS << ", ";
        clang::SourceRange ArgRange = CE->getArg(I)->getSourceRange();
        ArgsOS << clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(ArgRange),
            *Result.SourceManager, LangOpts);
      }
      ArgsOS.flush();
      
      //Perform mutation on the source code text by applying string replacement
      std::string NewCall = LambdaName + TemplateArgs + "(" + FuncArgs + ")";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FullRange, NewCall);
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxOperatorCallExpr(hasOperatorName("()")).bind("opCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}