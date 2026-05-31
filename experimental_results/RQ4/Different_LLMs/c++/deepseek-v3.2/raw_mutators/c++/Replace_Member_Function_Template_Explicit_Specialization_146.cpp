//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Function_Template_Explicit_Specialization_146
 */ 
class MutatorFrontendAction_146 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(146)

private:
    class MutatorASTConsumer_146 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_146(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Member_Function_Template_Explicit_Specialization_146.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Type.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/Stencil.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT146_OUTPUT 1

void MutatorFrontendAction_146::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<FunctionDecl>("ExplicitSpecialization")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      
      // Ensure it's a member function template explicit specialization defined inside a class
      if (!FS->isFunctionTemplateSpecialization() || !FS->getTemplateSpecializationInfo() || 
          !FS->getTemplateSpecializationInfo()->isExplicitSpecialization())
        return;
      
      // Check if it's defined inside a class (record)
      auto *RecordCtx = dyn_cast<CXXRecordDecl>(FS->getDeclContext());
      if (!RecordCtx || !RecordCtx->isCompleteDefinition())
        return;
      
      // Get the primary template
      FunctionTemplateDecl *PrimaryTemplate = FS->getTemplateSpecializationInfo()->getTemplate();
      if (!PrimaryTemplate)
        return;
      
      // Get template arguments of the specialization
      const TemplateArgumentList *TAList = FS->getTemplateSpecializationInfo()->TemplateArguments;
      if (!TAList || TAList->size() == 0)
        return;
      
      // Get the source range of the specialization
      SourceRange SpecializationRange = FS->getSourceRange();
      if (!SpecializationRange.isValid())
        return;
      
      // Get the body source
      std::string BodyStr;
      if (FS->hasBody()) {
        Stmt *Body = FS->getBody();
        SourceRange BodyRange = Body->getSourceRange();
        BodyStr = stringutils::rangetoStr(*(Result.SourceManager), BodyRange);
        // Remove braces if present
        if (BodyStr.size() >= 2 && BodyStr[0] == '{' && BodyStr[BodyStr.size()-1] == '}')
          BodyStr = BodyStr.substr(1, BodyStr.size()-2);
      } else {
        return; // No body to mutate
      }
      
      // Build condition based on template arguments
      std::string Condition;
      for (unsigned i = 0; i < TAList->size(); ++i) {
        const TemplateArgument &Arg = TAList->get(i);
        std::string ArgCondition;
        
        if (Arg.getKind() == TemplateArgument::Integral) {
          // Non-type template argument
          llvm::APSInt Val = Arg.getAsIntegral();
          ArgCondition = "N == " + Val.toString(10);
        } else if (Arg.getKind() == TemplateArgument::Type) {
          // Type template argument
          QualType Type = Arg.getAsType();
          std::string TypeStr = Type.getAsString();
          ArgCondition = "std::is_same<T, " + TypeStr + ">::value";
        } else if (Arg.getKind() == TemplateArgument::Template) {
          // Template template argument - skip for simplicity
          continue;
        } else {
          // Other kinds not handled
          continue;
        }
        
        if (!Condition.empty())
          Condition += " && ";
        Condition += ArgCondition;
      }
      
      if (Condition.empty())
        return;
      
      // Get return type
      std::string ReturnTypeStr = FS->getReturnType().getAsString();
      
      // Get function name
      std::string FuncName = FS->getNameAsString();
      
      // Get template parameters from primary template
      TemplateParameterList *TPList = PrimaryTemplate->getTemplateParameters();
      std::string TemplateParamsStr;
      if (TPList) {
        TemplateParamsStr = stringutils::rangetoStr(*(Result.SourceManager), 
                                                    TPList->getSourceRange());
      }
      
      // Construct mutated version with requires-clause (C++20 style)
      std::string MutatedCode;
      MutatedCode += "template<" + TemplateParamsStr + ">\n";
      MutatedCode += ReturnTypeStr + " " + FuncName + "() requires (" + Condition + ") {\n";
      MutatedCode += BodyStr + "\n";
      MutatedCode += "}";
      
      // Add comment to indicate mutation
      MutatedCode = "/*mut146*/" + MutatedCode;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SpecializationRange), MutatedCode);
    }
}
  
void MutatorFrontendAction_146::MutatorASTConsumer_146::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(
      isExplicitTemplateSpecialization(),
      isDefinition(),
      hasParent(cxxRecordDecl(isDefinition()))
    ).bind("ExplicitSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}