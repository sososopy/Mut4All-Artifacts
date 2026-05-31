//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Builtin_Va_Arg_With_Undeclared_Identifier_330
 */ 
class MutatorFrontendAction_330 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(330)

private:
    class MutatorASTConsumer_330 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_330(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_vardecls;
    };
};

//source file
#include "../include/Insert_Builtin_Va_Arg_With_Undeclared_Identifier_330.h"

// ========================================================================================================
#define MUT330_OUTPUT 1

void MutatorFrontendAction_330::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FB = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
      //Filter nodes in header files
      if (!FB || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FB->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto decls = FB->decls();
      for (auto decl : decls) {
        if (decl->getKind() == Decl::Kind::Var) {
          auto vardecl = dyn_cast<VarDecl>(decl);
          if (vardecl->getType().getAsString() == "va_list") {
            stringutils::printVarDeclInfo(vardecl);
            cur_vardecls.push_back(vardecl);
          }
        }
      }
      if (cur_vardecls.empty()) {
        for (auto decl : decls) {
          if (decl->getKind() == Decl::Kind::Var) {
            auto vardecl = dyn_cast<VarDecl>(decl);
            cur_vardecls.push_back(vardecl);
          }
        }
      }
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), FB->getSourceRange());
      llvm::outs() << content;
      //Perform mutation on the source code text by applying string replacement
      if (cur_vardecls.empty() == false) {
        auto target = cur_vardecls[0];
        auto targetname = target->getNameAsString();
        if (target->getType().getAsString() != "va_list") {
          auto target_type = target->getType().getAsString();
          llvm::outs() << target_type;
          auto target_range = target->getSourceRange();
          auto target_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                    target_range);
          llvm::outs() << target_str;
          target_str.replace(target_str.find(target_type), target_type.size(),
                             "va_list");
          llvm::outs() << target_str;
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(target->getSourceRange()),
              target_str);
        }
        llvm::outs() << targetname;
        auto insert_stmt = "__builtin_va_arg(" + targetname + ",int);\n";
        llvm::outs() << insert_stmt;
        content.insert(content.find('{') + 1, "/*mut330*/" + insert_stmt);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FB->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_330::MutatorASTConsumer_330::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = compoundStmt().bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}