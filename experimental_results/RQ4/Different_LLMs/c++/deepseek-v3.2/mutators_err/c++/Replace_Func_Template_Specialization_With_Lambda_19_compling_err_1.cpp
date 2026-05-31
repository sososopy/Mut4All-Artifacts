//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Func_Template_Specialization_With_Lambda_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Func_Template_Specialization_With_Lambda_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("decltypeAutoVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->hasInit())
        return;
      auto *Init = VD->getInit();
      if (!Init)
        return;
      if (!VD->getType()->getContainedAutoType())
        return;
      if (!VD->getType()->isDecltypeAutoType())
        return;
      auto *UE = dyn_cast<UnaryOperator>(Init);
      if (!UE)
        return;
      if (UE->getOpcode() != UO_AddrOf)
        return;
      auto *Ref = dyn_cast<DeclRefExpr>(UE->getSubExpr());
      if (!Ref)
        return;
      auto *FD = dyn_cast<FunctionDecl>(Ref->getDecl());
      if (!FD)
        return;
      if (!FD->isFunctionTemplateSpecialization())
        return;
      auto *TemplateArgs = FD->getTemplateSpecializationArgs();
      if (!TemplateArgs || TemplateArgs->size() == 0)
        return;
      //Get the source code text of target node
      SourceManager &SM = Rewrite.getSourceMgr();
      LangOptions LangOpts;
      auto InitRange = CharSourceRange::getTokenRange(UE->getSourceRange());
      std::string InitText = Lexer::getSourceText(InitRange, SM, LangOpts).str();
      //(optional)Record the node information to be used in the mutation process
      std::string LambdaText = "[";
      bool FirstArg = true;
      for (unsigned i = 0; i < TemplateArgs->size(); ++i) {
        if (!FirstArg)
          LambdaText += ", ";
        FirstArg = false;
        std::string ArgText;
        llvm::raw_string_ostream ArgStream(ArgText);
        TemplateArgs->get(i).print(LangOpts, ArgStream);
        ArgStream.flush();
        LambdaText += "template_arg" + std::to_string(i) + " = " + ArgText;
      }
      LambdaText += "]() -> decltype(auto) { return ";
      LambdaText += FD->getNameAsString() + "<";
      FirstArg = true;
      for (unsigned i = 0; i < TemplateArgs->size(); ++i) {
        if (!FirstArg)
          LambdaText += ", ";
        FirstArg = false;
        LambdaText += "template_arg" + std::to_string(i);
      }
      LambdaText += ">(";
      auto *PVD = FD->getParamDecl(0);
      if (PVD) {
        auto ParamType = PVD->getType();
        std::string DummyText;
        if (ParamType->isIntegerType())
          DummyText = "0";
        else if (ParamType->isFloatingType())
          DummyText = "0.0";
        else if (ParamType->isPointerType())
          DummyText = "nullptr";
        else
          DummyText = "{}";
        LambdaText += DummyText;
      }
      LambdaText += "); };";
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(InitRange, LambdaText);
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(decltypeAutoType()), hasInitializer(hasDescendant(unaryOperator(hasOperatorName("&"))))).bind("decltypeAutoVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}