//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Union_Designated_Initializer_With_Multiple_Field_Initializers_221
 */ 
class MutatorFrontendAction_221 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(221)

private:
    class MutatorASTConsumer_221 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_221(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> candidateVariables;
        std::vector<const clang::Expr*> candidateExpressions;
    };
};

//source file
#include "../include/Introduce_Union_Designated_Initializer_With_Multiple_Field_Initializers_221.h"

// ========================================================================================================
#define MUT221_OUTPUT 1

void MutatorFrontendAction_221::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto type = MT->getType()->getAsCXXRecordDecl();
      if (!type || !type->isUnion())
        return;
      auto init = MT->getInitializer();
      if (!init)
        return;
      auto initExpr = dyn_cast<CXXConstructExpr>(init);
      if (!initExpr)
        return;
      auto initList = initExpr->getNumArgs();
      if (initList != 1)
        return;
      auto arg = initExpr->getArg(0);
      if (!arg)
        return;
      //Record candidate variables and expressions from context
      for (auto decl : Result.Context->getTranslationUnitDecl()->decls()) {
        if (auto varDecl = dyn_cast<VarDecl>(decl)) {
          if (varDecl->hasGlobalStorage() || varDecl->isLocalVarDecl())
            candidateVariables.push_back(varDecl);
        }
      }
      for (auto stmt : Result.Context->getTranslationUnitDecl()->getStmtChildren()) {
        if (auto expr = dyn_cast<Expr>(stmt))
          candidateExpressions.push_back(expr);
      }
      //Perform mutation on the source code text by applying string replacement
      std::string newInit = "{";
      auto fields = type->fields();
      int count = 0;
      for (auto field : fields) {
        if (count >= 2)
          break;
        std::string fieldName = field->getNameAsString();
        std::string value;
        bool found = false;
        for (auto var : candidateVariables) {
          if (var->getType().getCanonicalType() == field->getType().getCanonicalType()) {
            value = var->getNameAsString();
            found = true;
            break;
          }
        }
        if (!found) {
          for (auto expr : candidateExpressions) {
            if (expr->getType().getCanonicalType() == field->getType().getCanonicalType()) {
              value = stringutils::rangetoStr(*(Result.SourceManager), expr->getSourceRange());
              found = true;
              break;
            }
          }
        }
        if (!found) {
          auto fieldType = field->getType();
          if (fieldType->isIntegerType())
            value = "0";
          else if (fieldType->isPointerType())
            value = "nullptr";
          else if (fieldType->isFloatingType())
            value = "0.0";
          else if (fieldType->isBooleanType())
            value = "false";
          else
            value = "{}";
        }
        newInit += "." + fieldName + "=" + value;
        if (count == 0)
          newInit += ", ";
        count++;
      }
      newInit += "}";
      std::string original = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      std::size_t initPos = original.find_last_of('{');
      if (initPos != std::string::npos) {
        original.replace(initPos, original.find_last_of('}') - initPos + 1, newInit);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), original);
    }
}
  
void MutatorFrontendAction_221::MutatorASTConsumer_221::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}