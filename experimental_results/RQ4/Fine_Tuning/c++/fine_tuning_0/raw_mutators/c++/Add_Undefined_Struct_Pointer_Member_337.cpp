//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Undefined_Struct_Pointer_Member_337
 */ 
class MutatorFrontendAction_337 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(337)

private:
    class MutatorASTConsumer_337 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_337(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Undefined_Struct_Pointer_Member_337.h"

// ========================================================================================================
#define MUT337_OUTPUT 1

void MutatorFrontendAction_337::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      //(optional)Record the node information to be used in the mutation process
      string undefined_struct = "Undefined";
      string undefined_decl = "struct " + undefined_struct + ";";
      string undefined_ptr =
          undefined_struct + " *" + "undef_ptr" + ";\n";
      //Perform mutation on the source code text by applying string replacement
      content.insert(0, undefined_decl);
      content.insert(content.rfind('}'), undefined_ptr);
      content = "/*mut337*/" + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_337::MutatorASTConsumer_337::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}