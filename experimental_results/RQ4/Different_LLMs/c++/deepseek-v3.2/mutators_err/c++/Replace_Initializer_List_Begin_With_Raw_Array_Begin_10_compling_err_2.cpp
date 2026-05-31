//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Initializer_List_Begin_With_Raw_Array_Begin_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Get the callee and check if it's a member call to begin()
      auto *Callee = CE->getCallee();
      if (!Callee) return;
      
      // Check if this is a call to a member function named "begin"
      auto *ME = dyn_cast<MemberExpr>(Callee->IgnoreParenImpCasts());
      if (!ME) return;
      
      auto *Member = ME->getMemberDecl();
      if (!Member || !Member->getIdentifier() || 
          !Member->getName().equals("begin")) return;
      
      // Get the object expression (the initializer_list)
      Expr *BaseExpr = ME->getBase();
      if (!BaseExpr) return;
      
      // Check if the base is a temporary initializer_list (CXXTemporaryObjectExpr)
      auto *TempObj = dyn_cast<CXXTemporaryObjectExpr>(BaseExpr->IgnoreParenImpCasts());
      if (!TempObj) return;
      
      // Check if the type is std::initializer_list
      QualType BaseType = TempObj->getType();
      if (!BaseType->isSpecificBuiltinType(BuiltinType::InitList)) return;
      
      // Get the template argument type (T in std::initializer_list<T>)
      const TemplateSpecializationType *TST = 
          dyn_cast<TemplateSpecializationType>(BaseType.getTypePtr());
      if (!TST || TST->getNumArgs() != 1) return;
      
      TemplateArgument Arg = TST->getArg(0);
      if (Arg.getKind() != TemplateArgument::Type) return;
      
      QualType ElementType = Arg.getAsType();
      if (ElementType.isNull()) return;
      
      //Get the source code text of target node
      SourceRange CallRange = CE->getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), CallRange);
      
      // Get the initializer_list expression text
      SourceRange InitListRange = TempObj->getSourceRange();
      std::string InitListText = stringutils::rangetoStr(*(Result.SourceManager), InitListRange);
      
      // Get the element type as string
      PrintingPolicy Policy(Result.Context->getLangOpts());
      std::string TypeStr;
      llvm::raw_string_ostream OS(TypeStr);
      ElementType.print(OS, Policy);
      OS.flush();
      
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = "(" + TypeStr + "*)" + InitListText + ".begin()";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CallRange, MutatedText);
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(memberExpr(member(hasName("begin")),
                                        hasObjectExpression(cxxTemporaryObjectExpr(
                                            hasType(templateSpecializationType(
                                                hasTemplateName(declRefExpr(to(
                                                    classTemplateDecl(hasName("initializer_list"))))))))))))
                                        .bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}