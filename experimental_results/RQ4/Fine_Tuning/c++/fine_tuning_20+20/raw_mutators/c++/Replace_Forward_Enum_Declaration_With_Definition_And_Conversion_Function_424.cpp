//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Forward_Enum_Declaration_With_Definition_And_Conversion_Function_424
 */ 
class MutatorFrontendAction_424 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(424)

private:
    class MutatorASTConsumer_424 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_424(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Forward_Enum_Declaration_With_Definition_And_Conversion_Function_424.h"

// ========================================================================================================
#define MUT424_OUTPUT 1

void MutatorFrontendAction_424::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isScoped() || MT->isCompleteDefinition())
        return;
      if (!MT->hasIntegerType())
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      auto underlying_type = MT->getIntegerType().getAsString();
      llvm::outs() << underlying_type << '\n';
      //Perform mutation on the source code text by applying string replacement
      content += "{Y=0,N=1}";
      string convert_type = "int";
      if (underlying_type == "int")
        convert_type = "bool";
      auto convert_func = "operator " + convert_type + "(" + MT->getNameAsString() +
                          " e){return (" + convert_type + ")e;}";
      content = "/*mut424*/" + content + convert_func;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_424::MutatorASTConsumer_424::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}