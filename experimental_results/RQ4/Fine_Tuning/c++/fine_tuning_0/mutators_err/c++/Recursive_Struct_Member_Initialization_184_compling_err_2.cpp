//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Struct_Member_Initialization_184
 */ 
class MutatorFrontendAction_184 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(184)

private:
    class MutatorASTConsumer_184 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_184(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Recursive_Struct_Member_Initialization_184.h"

// ========================================================================================================
#define MUT184_OUTPUT 1

void MutatorFrontendAction_184::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (!DL->isStruct())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto fields = DL->fields();
      if (fields.begin() == fields.end())
        return;
      auto field = *fields.begin();
      auto fieldname = field->getNameAsString();
      auto fieldtype =
          stringutils::rangetoStr(*(Result.SourceManager), field->getTypeSourceInfo()->getTypeLoc());
      llvm::outs() << fieldtype << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto ins = fieldtype + " f(const " + DL->getNameAsString() +
                 "& a){return " + fieldname + ";}\n";
      auto replace = fieldname + " = f(" + DL->getNameAsString() + "())";
      llvm::outs() << replace << '\n';
      if (content.find('}') != string::npos)
        content.insert(content.find('}'), replace);
      content = ins + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_184::MutatorASTConsumer_184::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}