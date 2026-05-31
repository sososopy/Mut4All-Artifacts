//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Initializer_List_Rvalue_With_Explicit_Temporary_Array_422
 */ 
class MutatorFrontendAction_422 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(422)

private:
    class MutatorASTConsumer_422 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_422(Rewriter &R) : TheRewriter(R) {}
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
#define MUT422_OUTPUT 1

void MutatorFrontendAction_422::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      const Expr *callee = MT->getImplicitObjectArgument();
      if (!callee) return;
      
      // Check if the implicit object argument is an initializer list temporary
      if (const MaterializeTemporaryExpr *tempExpr = dyn_cast<MaterializeTemporaryExpr>(callee)) {
        const Expr *tempSubExpr = tempExpr->getSubExpr();
        if (const CXXTemporaryObjectExpr *tempObjExpr = dyn_cast<CXXTemporaryObjectExpr>(tempSubExpr)) {
          // Check if it's constructing std::initializer_list
          const CXXRecordDecl *record = tempObjExpr->getType()->getAsCXXRecordDecl();
          if (!record || record->getName() != "initializer_list") return;
          
          // Check if the constructor argument is a list
          if (tempObjExpr->getNumArgs() == 1) {
            const Expr *arg = tempObjExpr->getArg(0);
            if (const InitListExpr *initList = dyn_cast<InitListExpr>(arg)) {
              //Get the source code text of target node
              std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
              
              // Determine common type and count
              QualType commonType = initList->getType();
              if (commonType.isNull()) return;
              unsigned count = initList->getNumInits();
              
              // Get the element type (remove const if present in array context)
              QualType elemType = commonType;
              if (elemType.isConstQualified()) {
                elemType = elemType.getNonConstQualifiedType();
              }
              
              std::string elemTypeStr = elemType.getAsString();
              std::string arrayType = "const " + elemTypeStr + "[" + std::to_string(count) + "]";
              
              // Build array initializer text
              std::string initText;
              for (unsigned i = 0; i < count; ++i) {
                const Expr *initExpr = initList->getInit(i);
                std::string initStr = stringutils::rangetoStr(*(Result.SourceManager), initExpr->getSourceRange());
                if (i > 0) initText += ", ";
                initText += initStr;
              }
              
              // Build mutated expression
              std::string mutatedText = "const " + elemTypeStr + " (&arr)[" + std::to_string(count) + "] = {" + initText + "}; ";
              mutatedText += "std::initializer_list<" + elemTypeStr + ">(arr)." + MT->getDirectCallee()->getNameInfo().getName().getAsString() + "()";
              
              //Perform mutation on the source code text by applying string replacement
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_422::MutatorASTConsumer_422::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = memberCallExpr(
      has(
        implicitObjectArgument(
          materializeTemporaryExpr(
            has(
              cxxTemporaryObjectExpr(
                hasType(
                  recordDecl(
                    hasName("initializer_list")
                  )
                ),
                hasArgument(
                  0,
                  initListExpr()
                )
              )
            )
          )
        )
      )
    ).bind("MemberCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}