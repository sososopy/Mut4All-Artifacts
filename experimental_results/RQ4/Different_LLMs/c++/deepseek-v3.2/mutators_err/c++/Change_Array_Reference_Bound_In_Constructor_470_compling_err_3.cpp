//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Array_Reference_Bound_In_Constructor_470
 */ 
class MutatorFrontendAction_470 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(470)

private:
    class MutatorASTConsumer_470 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_470(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<int> availableConstants;
        std::vector<const clang::VarDecl*> constantDecls;
    };
};

//source file
#include "../include/Mutator_470.h"

// ========================================================================================================
#define MUT470_OUTPUT 1

void MutatorFrontendAction_470::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if field is a reference to array
      const clang::Type* fieldType = FD->getType().getTypePtr();
      if (!fieldType->isReferenceType()) return;
      
      const clang::Type* referencedType = fieldType->getPointeeType().getTypePtr();
      if (!referencedType->isArrayType()) return;
      
      const clang::ArrayType* arrayType = llvm::dyn_cast<clang::ArrayType>(referencedType);
      if (!arrayType) return;
      
      const clang::ConstantArrayType* constArrayType = llvm::dyn_cast<clang::ConstantArrayType>(arrayType);
      if (!constArrayType) return;
      
      int originalBound = constArrayType->getSize().getLimitedValue();
      
      // Find constructor initializer for this field
      const clang::RecordDecl* recordDecl = FD->getParent();
      if (!recordDecl) return;
      
      const clang::CXXRecordDecl* cxxRecordDecl = llvm::dyn_cast<clang::CXXRecordDecl>(recordDecl);
      if (!cxxRecordDecl) return;
      
      // Collect available constants from the translation unit
      if (availableConstants.empty()) {
        auto& SM = Result.Context->getSourceManager();
        auto decls = Result.Context->getTranslationUnitDecl()->decls();
        for (auto decl : decls) {
          if (auto* VD = llvm::dyn_cast<clang::VarDecl>(decl)) {
            if (!SM.isWrittenInMainFile(VD->getLocation())) continue;
            if (VD->getType()->isIntegerType() && VD->isConstexpr()) {
              if (auto* ICE = VD->getInit()) {
                if (auto* IL = llvm::dyn_cast<clang::IntegerLiteral>(ICE)) {
                  int value = IL->getValue().getLimitedValue();
                  if (value > 0 && value != originalBound) {
                    availableConstants.push_back(value);
                    constantDecls.push_back(VD);
                  }
                }
              }
            }
          }
        }
      }
      
      // Find constructor for this record
      for (auto* method : cxxRecordDecl->methods()) {
        if (auto* ctor = llvm::dyn_cast<clang::CXXConstructorDecl>(method)) {
          if (!Result.Context->getSourceManager().isWrittenInMainFile(ctor->getLocation())) continue;
          
          // Check initializer list
          for (auto init = ctor->init_begin(); init != ctor->init_end(); ++init) {
            if (init->getMember() == FD) {
              // Get initializer expression type
              clang::Expr* expr = init->getInit();
              if (!expr) continue;
              
              clang::QualType exprType = expr->getType();
              const clang::Type* exprTypePtr = exprType.getTypePtr();
              
              // Check if expression yields array type
              const clang::ArrayType* exprArrayType = nullptr;
              if (exprTypePtr->isArrayType()) {
                exprArrayType = llvm::dyn_cast<clang::ArrayType>(exprTypePtr);
              } else if (exprTypePtr->isPointerType()) {
                const clang::Type* pointeeType = exprTypePtr->getPointeeType().getTypePtr();
                if (pointeeType->isArrayType()) {
                  exprArrayType = llvm::dyn_cast<clang::ArrayType>(pointeeType);
                }
              }
              
              if (!exprArrayType) continue;
              
              const clang::ConstantArrayType* exprConstArrayType = llvm::dyn_cast<clang::ConstantArrayType>(exprArrayType);
              if (!exprConstArrayType) continue;
              
              int exprBound = exprConstArrayType->getSize().getLimitedValue();
              
              // Choose a different bound
              int newBound = -1;
              if (!availableConstants.empty()) {
                size_t index = getrandom::getRandomIndex(availableConstants.size() - 1);
                newBound = availableConstants[index];
              } else {
                // Create a new constant if none available
                newBound = exprBound + 1;
              }
              
              if (newBound == exprBound) {
                newBound = exprBound + 1;
              }
              
              //Get the source code text of target node
              auto fieldSourceRange = FD->getSourceRange();
              if (!fieldSourceRange.isValid()) continue;
              
              std::string fieldText = stringutils::rangetoStr(*(Result.SourceManager), fieldSourceRange);
              
              // Find the array bound in the field declaration
              size_t openBracketPos = fieldText.find('[');
              if (openBracketPos == std::string::npos) continue;
              size_t closeBracketPos = fieldText.find(']', openBracketPos);
              if (closeBracketPos == std::string::npos) continue;
              
              // Extract the bound substring
              std::string boundStr = fieldText.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1);
              
              //Perform mutation on the source code text by applying string replacement
              std::string newBoundStr = std::to_string(newBound);
              std::string mutatedFieldText = fieldText;
              mutatedFieldText.replace(openBracketPos + 1, boundStr.length(), newBoundStr);
              
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(fieldSourceRange, mutatedField2Text);
              
              // If no existing constant was used, add a constexpr variable in constructor body
              if (availableConstants.empty() && ctor->hasBody()) {
                std::string constDecl = "constexpr int mut_470_bound = " + std::to_string(newBound) + ";";
                clang::CompoundStmt* body = llvm::dyn_cast<clang::CompoundStmt>(ctor->getBody());
                if (body) {
                  SourceLocation insertLoc = body->getBeginLoc();
                  Rewrite.InsertTextBefore(insertLoc, "\n/*mut470*/" + constDecl + "\n");
                }
              }
              
              return;
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_470::MutatorASTConsumer_470::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasType(referenceType(pointee(isArrayType())))).bind("FieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}